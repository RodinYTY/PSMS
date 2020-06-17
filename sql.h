#ifndef SQL_H
#define SQL_H
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

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

    /*-------------------------*/

    const QString rootpwd = "yty02036054866";

    //建表
    QString createTables = "CREATE TABLE IF NOT EXISTS `student`(\
               `sno` int NOT NULL AUTO_INCREMENT,\
               `sname` char(10) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,\
               `sid` char(18) NOT NULL,\
               `ssex` char(1) NOT NULL,\
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
               `tsex` char(1) NOT NULL,\
               `tage` smallint NOT NULL,\
               `tphone` char(11) DEFAULT NULL,\
               `tmajor` char(8) DEFAULT NULL,\
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
    QString createRoles = "CREATE role teachers;\
            GRANT SELECT, UPDATE ON TABLE\
                course TO teachers;\
            GRANT SELECT ON TABLE\
                student TO teachers;\
            GRANT SELECT ON TABLE\
                teacher TO teachers;\
            GRANT SELECT ON TABLE\
                room TO teachers;\
            CREATE role students;\
            GRANT SELECT ON TABLE\
                course TO students;\
            GRANT SELECT ON TABLE\
                student TO students;\
            GRANT SELECT ON TABLE\
                teacher TO students;\
            GRANT SELECT ON TABLE\
                room TO students;";

    //创建用户
    QString createUser = "create user %1@%2 identified by \'%3\';";
    QString studentGranting = "grant students to %1@%2;";
    QString teacherGranting = "grant teachers to %1@%2;";

    //学生：姓名 身份证 性别 年龄 手机号 账户名
    QString insertIntoStudent = "insert into student\
            values(\'%1\', \'%2\', \'%3\', \'%4\', \'%5\', \'%6\');";
    QString createTrigger = "CREATE TRIGGER insert_student AFTER INSERT ON `student` FOR EACH ROW\
            BEGIN\
                DELETE FROM CODE \
                WHERE\
                    datediff( now( ), genDate ) > 7;\
            END;";
    //老师：姓名 身份证 性别 年龄 手机号 专业 账户名
    QString insertIntoTeacher = "insert into teacher\
            values(\'%1\', \'%2\', \'%3\', %4, \'%5\', \'%6\', \'%7\');";
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



};

#endif // SQL_H