#include "log.h"


Log::Log()
    :m_bEnabled(true)
{
}

Log::~Log()
{
}

bool Log::Open(std::string sFileName)
{
    m_tOLogFile.open(sFileName.c_str(), std::ios_base::out | std::ios_base::app);

    if( !m_tOLogFile )
    {
        return false;
    }
    this->m_fileName==sFileName;
    return true;
}

void Log::Close()
{
    if(m_tOLogFile.is_open())
    {
        m_tOLogFile.close();
    }
}

bool Log::CommonLogInit()
{
    time_t tNowTime;
    time(&tNowTime);

    tm* tLocalTime = localtime(&tNowTime);

    //得到日期的字符串
    std::string sDateStr = ValueToStr(tLocalTime->tm_year+1900) + "-" +
        ValueToStr(tLocalTime->tm_mon+1) + "-" +
        ValueToStr(tLocalTime->tm_mday);
    std::string strFileName="Log_" + sDateStr + ".log";
    this->m_fileName=strFileName;

    return Open("Log_" + sDateStr + ".log");
}

void Log::Clear()
{
    if(m_tOLogFile.is_open())
    {
        m_tOLogFile.close();
        m_tOLogFile.open(m_fileName.c_str(),std::ios::trunc);
        m_tOLogFile.close();
        m_tOLogFile.open(m_fileName.c_str(), std::ios_base::out | std::ios_base::app);
    }
    else
    {
        m_tOLogFile.open(m_fileName.c_str(),std::ios::trunc);
        m_tOLogFile.close();
    }
//    std::ofstream ofs;
//    ofs.open(m_fileName.c_str(), std::ofstream::out | std::ofstream::trunc);
//    ofs.close();
    //this->m_tOLogFile(this->m_fileName,std::ios::trunc);
    //std::ofstream ofs(this->m_fileName, std::ofstream::out | std::ofstream::trunc);
    //std::ofstream ofs(this->m_tOLogFile,std::ios::trunc);
}

void Log::Enable()
{
    m_bEnabled = true;
}

void Log::Disable()
{
    m_bEnabled = false;
}

//得到当前时间的字符串
std::string Log::GetTimeStr()
{
    time_t tNowTime;
    time(&tNowTime);

    tm* tLocalTime = localtime(&tNowTime);

    //"2011-07-18 23:03:01 ";
    //std::string strFormat = "%Y-%m-%d %H:%M:%S ";
    std::string strFormat = "%H:%M:%S ";

    char strDateTime[30] = {'\0'};
    strftime(strDateTime, 30, strFormat.c_str(), tLocalTime);

    std::string strRes = strDateTime;

    return strRes;
}
