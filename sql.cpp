#include "sql.h"

SQL::SQL()
{

}

QString SQL::major_to_chinese(major_set index){
    QString ret;
    switch(index){
    case piano:
        ret = "钢琴";
        break;
    case violin:
        ret = "小提琴";
        break;
    case guitar:
        ret = "吉他";
        break;
    case drum:
        ret = "架子鼓";
        break;
    case bass:
        ret = "贝斯";
        break;
    case guzheng:
        ret = "古筝";
        break;
    case ukulele:
        ret = "尤克里里";
        break;
    }
    return ret;
}
