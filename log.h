#ifndef LOG_H
#define LOG_H

#include<fstream>
#include<string>
#include<sstream>
#include<ctime>
#include<string>


class Log
{
public:
    Log();
    ~Log();

    bool Open(std::string strFileName);
    void Close();

    bool CommonLogInit(); //打开默认的log 文件
    void Clear();

    void Enable();
    void Disable();
private:
    std::ofstream m_tOLogFile;

    bool m_bEnabled;

    std::string m_fileName;
public:
    std::string GetTimeStr();

    template <typename T> void LogOut(const T& value)
    {
        if (m_bEnabled)
        {
            m_tOLogFile << value;
        }
    }

    template <typename T> void LogOutLn(const T& value)
    {
        if (m_bEnabled)
        {
            m_tOLogFile << value << std::endl;
        }
    }

    void LogOutLn()
    {
        if (m_bEnabled)
        {
            m_tOLogFile << std::endl;
        }
    }

    template <typename T> Log& operator<<(const T& value)
    {
        if (m_bEnabled)
        {
            m_tOLogFile << value;
        }
        return (*this);
    }

//    Log& operator<<(ostream& (*_Pfn)(ostream&))
//    {
//        if (m_bEnabled)
//        {
//            (*_Pfn)(m_tOLogFile);
//        }
//        return (*this);
//    }

public:
    template<typename T> std::string ValueToStr(T value)
    {
        std::ostringstream ost;
        ost << value;
        return ost.str();
    }

};

#endif // LOG_H
