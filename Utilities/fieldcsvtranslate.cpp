#include "fieldcsvtranslate.h"

std::string procon::json::translateToFieldCsv(std::string json_str){

}
std::string procon::json::translateFromFieldCsv(std::string field_csv_str){

}

procon::Field procon::json::importJsonToField(std::string path){

    std::ifstream ifs(path);
    std::istreambuf_iterator<char> it(ifs);
    std::istreambuf_iterator<char> last;
    std::string str(it, last);
    std::string csv_str = procon::json::translateToFieldCsv(str);// ファイルの中身をtranslateToFieldCsvに投げて, 結果を取得する

    procon::Field field = csv::csvDecode(csv_str);// 出てきたcsv形式の文字列をprocon::Field形式に変換して, returnする
    return field;
}
