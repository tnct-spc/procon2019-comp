#include "fieldcsvtranslate.h"

std::string procon::json::translateToFieldCsv(std::string json_str){

}
std::string procon::json::translateFromFieldCsv(std::string field_csv_str){

}

procon::Field procon::json::importJsonToField(std::string path){

}

int main()
{

        std::ifstream ifs("フィールド情報_turn0.json");
        std::string str;
        std::istreambuf_iterator<char> it(ifs);
            std::istreambuf_iterator<char> last;
            std::string str(it, last);
       procon::json::translateFromFieldCsv(str);// ファイルの中身をtranslateFromFieldCsvに投げて, 結果を取得する
}

    // ファイルの中身をtranslateFromFieldCsvに投げて, 結果を取得する

    // 出てきたcsv形式の文字列をprocon::Field形式に変換して, returnする

