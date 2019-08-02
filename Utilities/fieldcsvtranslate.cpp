#include "fieldcsvtranslate.h"

std::string procon::json::translateToFieldCsv(std::string json_str, int team_id, int end_turn){

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
    add_integer (j["teams"][0]["tilePoint"]);//0タイル点
    add_integer (j["teams"][0]["areaPoint"]);//０領域点
    add_integer (j["teams"][1]["tilePoint"]);//1タイル点
    add_integer (j["teams"][1]["areaPoint"]);//1領域点
    int a = j["teams"][0]["agents"];//チーム０のエージェント数
    add_integer(a);//エージェント数
    for(int i = 0; i < a; ++i){
        int team_id_0 = j["teams"][0]["teamID"];
        int x0 = j["teams"][0]["agents"][i]["x"];
        int y0 = j["teams"][0]["agents"][i]["y"];
        int pos_0 = pointToInt(procon::Point(x0, y0));
        int x1 = j["teams"][1]["agents"][i]["x"];
        int y1 = j["teams"][1]["agents"][i]["y"];
        int pos_1 = pointToInt(procon::Point(x1, y1));
        if(team_id_0 == team_id){// team_id_0 == team_id: 先に追加する
            add_integer (pos_0);//team0を追加
            add_integer (pos_1);//team1を追加
        }else {// team_id_0 != team_id:後に追加する
            add_integer (pos_1);//team1を追加
            add_integer (pos_0);//team0を追加
        }
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
std::string procon::json::translateFromFieldCsv(std::string field_csv_str){

}

procon::Field procon::json::importJsonToField(std::string path, int team_id, int end_turn){

    std::ifstream ifs(path);
    std::istreambuf_iterator<char> it(ifs);
    std::istreambuf_iterator<char> last;
    std::string str(it, last);
    std::string csv_str = procon::json::translateToFieldCsv(str, team_id, end_turn);// ファイルの中身をtranslateToFieldCsvに投げて, 結果を取得する

    procon::Field field = csv::csvDecode(csv_str);// 出てきたcsv形式の文字列をprocon::Field形式に変換して, returnする
    return field;
}
