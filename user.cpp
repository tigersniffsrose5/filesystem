#include "user.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <stdlib.h>

using namespace std;
namespace fs = std::filesystem;

const fs::path userListFilePath("./users");
const fs::path currentUserFilePath("./current_user");                   //当前用户
const string rootUserName("root");                                      
constexpr int rootUID = 0;                                              //constexpr特指编译期常量
constexpr int defaultUserUID = 1;

istream &operator>>(istream &input, user &u)                            //重载>>
{
    input >> u.uid >> ws;                                               //忽略空格
    return getline(input, u.name);
}

ostream &operator<<(ostream &output, const user &u)                     //重载<< 
{
    return output << u.uid << " " << u.name;
}

void doLogin(ffsuid_t uid)                                              //将登录的用户加入到文件    
{
    ofstream(currentUserFilePath) << uid;
}

int login(int argc, char *argv[])
{
    if ( argc == 0 ) {

        cerr << "ERROR 请指定用户名" << endl;
        return 1;
    
    }

    auto user = getUser(argv[0]);
    doLogin(user.uid);

    return 0;
}

bool hasUserInit()                                                       
{
    return fs::exists(userListFilePath) && fs::exists(currentUserFilePath);
}

void initUser()
{
    if ( hasUserInit() ) 
        return;

    ofstream users(userListFilePath);
    users << user{rootUID, rootUserName} << endl;
    int loginUID = rootUID;

    char *systemUser = getenv("LOGNAME");                               //搜索环境字符串,获取当前登录用户

    if ( systemUser != nullptr && systemUser != rootUserName ) {

        users << user{defaultUserUID, systemUser} << endl;
        loginUID = defaultUserUID;
    
    }

    doLogin(loginUID);
}

user currentUser()
{
    ffsuid_t uid;
    ifstream(currentUserFilePath) >> uid;
    return getUser(uid);
}

user getUser(ffsuid_t uid)
{
    ifstream users(userListFilePath);
    user u;
    
    while ( users >> u.uid ) {
        
        if ( u.uid == uid ) {
        
            users >> u.name;
            return u;
        
        }

        users >> u.name;
    
    }

    stringstream errMsg;
    errMsg << "uid " << uid << " not exists";
    throw out_of_range(errMsg.str());
}

user getUser(string name)
{
    ifstream users(userListFilePath);
    user u;

    while ( users >> u.uid ) {

        users >> u.name;
        
        if ( u.name == name )
            return u;

    }

    stringstream errMsg;
    errMsg << "user name " << name << " not exists";
    throw out_of_range(errMsg.str());
}

int whoami(int argc, char *argv[])
{
    cout << currentUser().name << endl;
    return 0;
}

int adduser(int argc, char *argv[])
{
    if ( argc <= 0 )
        throw runtime_error("缺少参数：要添加的用户");

    if ( currentUser().uid != rootUID )
        throw runtime_error("only root can add user.");

    fstream users(userListFilePath);
    user u;
    ffsuid_t lastUid = 0;

    while ( users >> u.uid ) {

        users >> u.name;

        if ( u.name == argv[0] )
            throw runtime_error("user already exists.");
        
        lastUid = u.uid;

    }

    user newUser;
    newUser.uid = lastUid + 1;
    newUser.name = argv[0];
    users.clear();
    users.seekp(0, ios::end);
    users << newUser << endl;

    return 0;
}

bool isRoot(ffsuid_t uid)
{
    return uid == rootUID;
}
