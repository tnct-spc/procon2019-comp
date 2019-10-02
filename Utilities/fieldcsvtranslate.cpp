#include "fieldcsvtranslate.h"

std::string procon::json::translateToFieldCsv(std::string json_str, int team_id, std::vector<int>& agent_ids, int end_turn){

    std::string csv_str = "";
    auto add_integer = [&csv_str](int number){
        if(csv_str != "")
            csv_str += ",";
        csv_str += std::to_string(number);
    };

    nlohmann::json j = nlohmann::json::parse(json_str);
    int size_x = j["width"];//フィールド横幅
    int size_y = j["height"];//フィールド縦幅
    auto pointToInt = [&size_y](auto p){return p.x * size_y + p.y;};
    add_integer (size_x);
    add_integer (size_y);
    add_integer (j["turn"]);//現在ターン数
    add_integer (end_turn);//終了ターン数

    bool my_id = (j["teams"][0]["teamID"] != team_id);
    if(j["teams"][my_id]["teamID"] != team_id)
        return "team_error";

    add_integer (j["teams"][my_id]["tilePoint"]);//0タイル点
    add_integer (j["teams"][my_id]["areaPoint"]);//０領域点
    add_integer (j["teams"][!my_id]["tilePoint"]);//1タイル点
    add_integer (j["teams"][!my_id]["areaPoint"]);//1領域点

    int agent_count = j["teams"][0]["agents"].size();//チーム０のエージェント数
    add_integer (agent_count);

    std::vector<nlohmann::json> agent_datas(agent_count);

    for(int i = 0; i < agent_count; ++i){
        bool agent_found_flag = false;
        for(int k = 0; k < agent_count; ++k){
            int agent_id = j["teams"][my_id]["agents"][k]["agentID"];
            if(agent_id == agent_ids.at(i)){
                agent_datas[i] = j["teams"][my_id]["agents"][k];
                agent_found_flag = true;
                break;
            }
        }
        if(!agent_found_flag)
            return "agent_error";
    }
    for(int i = 0; i < agent_count; ++i){
        auto& data_json = agent_datas[i];
        int x = data_json["x"];
        int y = data_json["y"];
        int pos = pointToInt(procon::Point(x - 1, y - 1));
        add_integer (pos);
    }
    for(int i = 0; i < agent_count; ++i){
        auto& data_json = j["teams"][!my_id]["agents"][i];
        int x = data_json["x"];
        int y = data_json["y"];
        int pos = pointToInt(procon::Point(x - 1, y - 1));
        add_integer (pos);
    }

    for(int x = 0; x < size_x; ++x){
        for(int y = 0; y < size_y; ++y){
            int value = j["points"][x][y];
            int tile_team_id = j["tiled"][x][y];
            int tile;
            if(tile_team_id == 0){
                tile=0;// 0
            }else if(tile_team_id == team_id){
                tile=1;// 1
            }else{
                tile=2;// 2
            }

            add_integer(tile);
            add_integer(value);
            add_integer(0);
            add_integer(0);
        }
    }

    procon::Field field = csv::csvDecode(csv_str);// csv_strをFieldに変換 81のコードができてこっちができないのがわからん
    field.calcRegionPoint();// Field内で領域点を計算 引数をなんか使うのかわからん
    csv_str = csv::csvEncode(field) ; // Fieldをcsv形式に変換
    return csv_str;// csvをreturnする
}

std::vector<int> procon::json::getAgentIds(std::string json_str, int team_id){
    nlohmann::json j = nlohmann::json::parse(json_str);
    bool my_id = (j["teams"][0]["teamID"] != team_id);
    if(j["teams"][my_id]["teamID"] != team_id){
        return std::vector<int>(0);
    }
    auto agents_json = j["teams"][my_id]["agents"];
    int agent_count = agents_json.size();//チーム０のエージェント数
    std::vector<int> agent_ids(agent_count, -1);
    for(int i = 0; i < agent_count; ++i){
        agent_ids.at(i) = agents_json.at(i)["agentID"];
    }
    std::cout << "update agent_id" << std::endl;
    return agent_ids;
}

std::string procon::json::translateFromFieldCsv(std::string field_csv_str){

}

procon::Field procon::json::importJsonToField(std::string path, int team_id, std::vector<int>& agent_ids, int end_turn){

    std::ifstream ifs(path);
    std::istreambuf_iterator<char> it(ifs);
    std::istreambuf_iterator<char> last;
    std::string str(it, last);
    std::string csv_str = procon::json::translateToFieldCsv(str, team_id, agent_ids, end_turn);// ファイルの中身をtranslateToFieldCsvに投げて, 結果を取得する

    procon::Field field = csv::csvDecode(csv_str);// 出てきたcsv形式の文字列をprocon::Field形式に変換して, returnする
    return field;
}
