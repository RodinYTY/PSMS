#ifndef SQL_H
#define SQL_H
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QInputDialog>
#include <QSqlTableModel>


class SQL
{
public:
    SQL();


    //连接数据结构
    struct Configuration{
        QString _dbname, _linkname, _port, _usrname, _pwd;
        int _rem, _auto;

    public:
        void print(){
            qDebug() << "数据库名 主机名 端口号 用户名 密码 记住密码 自动登录";
            qDebug() << _dbname << _linkname << _port << _usrname << _pwd <<_rem << _auto;
        }
    };

    enum major_set{
        piano,violin,guitar,drum,bass,guzheng,ukulele
    };

    struct UserInfo{
        QString usrname, pwd, name, phone, id, code;
        int age, sex, isTeacher;
        major_set major;
    };//注册信息

    /*-----------方法-----------*/
    QString major_to_chinese(major_set);

    int isDigitStr(QString src)
    {
        QByteArray ba = src.toLatin1();//QString 转换为 char*
         const char *s = ba.data();

        while(*s && *s>='0' && *s<='9') s++;

        if (*s)
        { //不是纯数字
            return 0;
        }
        else
        { //纯数字
            return 1;
        }
    }

    /*-------------------------*/

    QString rootpwd;

    //建表
    QString createTables = "CREATE TABLE IF NOT EXISTS `student`(\
               `sno` int NOT NULL AUTO_INCREMENT,\
               `sname` char(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
               `sid` char(18) NOT NULL,\
               `ssex` enum(\'男\',\'女\'),\
               `sage` smallint NOT NULL,\
               `sphone` char(11) DEFAULT NULL,\
               `uname` char(20) DEFAULT NULL,\
               PRIMARY KEY (`sno`)\
             );\
             \
             CREATE TABLE IF NOT EXISTS `teacher`(\
               `tno` int NOT NULL AUTO_INCREMENT,\
               `tname` char(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
               `tid` char(18) NOT NULL,\
               `tsex` enum(\'男\',\'女\'),\
               `tage` smallint NOT NULL,\
               `tphone` char(11) DEFAULT NULL,\
               `tmajor` enum(\'钢琴\',\'小提琴\',\'吉他\',\'鼓\',\'贝斯\',\'古筝\',\'尤克里里\'),\
               `uname` char(20) DEFAULT NULL,\
               PRIMARY KEY (`tno`)\
             );\
             \
             CREATE TABLE IF NOT EXISTS `room`(\
               `rno` int NOT NULL AUTO_INCREMENT,\
               `rarea` int DEFAULT NULL,\
               `rrent` int DEFAULT NULL,\
               PRIMARY KEY (`rno`)\
             );\
             \
             CREATE TABLE IF NOT EXISTS `course`(\
               `sno` int NOT NULL,\
               `tno` int NOT NULL,\
               `rno` int NOT NULL,\
               `stime` datetime NOT NULL,\
               `atime` datetime NOT NULL,\
               `fee` int NOT NULL,\
               KEY `sno` (`sno`),\
               KEY `tno` (`tno`),\
               KEY `rno` (`rno`),\
               CONSTRAINT `course_ibfk_1` FOREIGN KEY (`sno`) REFERENCES `student` (`sno`) ON DELETE CASCADE ON UPDATE CASCADE,\
               CONSTRAINT `course_ibfk_2` FOREIGN KEY (`tno`) REFERENCES `teacher` (`tno`) ON DELETE CASCADE ON UPDATE CASCADE,\
               CONSTRAINT `course_ibfk_3` FOREIGN KEY (`rno`) REFERENCES `room` (`rno`) ON DELETE CASCADE ON UPDATE CASCADE\
             );\
             \
             CREATE TABLE IF NOT EXISTS `instrument`(\
               `ino` int NOT NULL AUTO_INCREMENT,\
               `rno` int NOT NULL,\
               `imodel` varchar(30) DEFAULT NULL,\
               `iprice` int DEFAULT NULL,\
               PRIMARY KEY (`ino`),\
               KEY `rno` (`rno`),\
               CONSTRAINT `instrument_ibfk_1` FOREIGN KEY (`rno`) REFERENCES `room` (`rno`) ON DELETE CASCADE ON UPDATE CASCADE\
             );\
             \
             CREATE TABLE IF NOT EXISTS `code` (\
               `icode` char(6) NOT NULL,\
               `genDate` date NOT NULL,\
               PRIMARY KEY (`icode`)\
             );\
             \
             SET GLOBAL log_bin_trust_function_creators = TRUE;\
             \
             CREATE FUNCTION genCode ( n INT ) RETURNS VARCHAR ( 1024 ) BEGIN\
                 DECLARE\
                     result VARCHAR ( 1024 );\
                 SELECT\
                     substring( MD5( RAND( ) ), 1, n ) INTO result;\
                 RETURN result;\
             END;";

    //创建学生和老师角色
    QString teacherGranting = "GRANT ALL PRIVILEGES ON TABLE course TO %1@%2;\
            GRANT SELECT ON TABLE student TO %1@%2;\
            GRANT SELECT, UPDATE ON TABLE teacher TO %1@%2;\
            GRANT SELECT ON TABLE room TO %1@%2;";

    QString studentGranting = "GRANT SELECT ON TABLE course TO %1@%2;\
            GRANT SELECT, UPDATE ON TABLE student TO %1@%2;\
            GRANT SELECT ON TABLE teacher TO %1@%2;\
            GRANT SELECT ON TABLE room TO %1@%2;";

//创建用户
QString createUser = "create user %1@%2 identified by \'%3\';";

//学生：姓名 身份证 性别 年龄 手机号 账户名
QString createTrigger = "CREATE TRIGGER insert_student AFTER INSERT ON `student` FOR EACH ROW\
        BEGIN\
            DELETE FROM CODE \
            WHERE\
                datediff( now( ), genDate ) > 7;\
        END;";

//带主码
QString insertIntoStudentWithIndex = "insert into student\
        values(%1, \'%2\', \'%3\', \'%4\', %5, \'%6\', \'%7\');";
QString insertIntoTeacherWithIndex = "insert into teacher\
        values(%1, \'%2\', \'%3\', \'%4\', %5, \'%6\', \'%7\', \'%8\');";

//清除7天前的邀请码
QString clearCodes = "DELETE FROM CODE WHERE datediff( now( ), genDate ) > 7;";
//查询邀请码
QString selectCodeOf = "select count(*) from code where icode = \'%1\';";
//删除邀请码
QString delectCodeOf = "delete from code where icode = \'%1\';";
//插入邀请码
QString insertCodeOf = "insert into code values (\'%1\', now());";

//查找账户名
QString acountInStudent = "select count(*) from student where uname=\'%1\';";
QString acountInTeacher = "select count(*) from teacher where uname=\'%1\';";

QString searchTeacher = "tno = %1 or tno in (select tno from teacher where tid like \'%%1%\' or tphone like \'%%1%\' or uname like \'%%1%\')";
QString searchStudent = "sno = %1 or sno in (select sno from student where sid like \'%%1%\' or sphone like \'%%1%\' or uname like \'%%1%\')";



};

#endif // SQL_H
