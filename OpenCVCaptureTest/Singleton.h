/** file
*Copyright(C):	Xiamen Kelude Technology Co Ltd., All rights reserved.
*n
*n 文件:Singleton.h
*n 功能:定义单件模式模板类
*n 作者:张秋水(zhangqiushui@xm-kld.com) 2020-02-10
*n 修改：
*/
#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
using namespace std;

#include <QMutex>

/**
* brief
* 封装针对字符串的安全操作。
* n 典型用法：
* #include "Singleton.h"
*
* //CConfigMgr是需要实现单件模式的类
* class CConfigMgr
*{
*public:
*void Run()
*{
*
*}
*private:
*CConfigMgr(void)
*{
*
*}
*virtual ~CConfigMgr(void)
*{
*
*}
*DECLARE_SINGLETON_CLASS(CConfigMgr);
*};
*
*typedef CSingleton<CConfigMgr> theConfigMgr;
*
*在使用的时候很简单，跟一般的Singleton实现的方法没有什么不同。
*
*int _tmain(int argc, _TCHAR* argv[])
*{
*	theConfigMgr::instance()->Run();
*}
* n 注意事项：
* n 包括：
* n 作者：张秋水(zhangqiushui@qq.com)
* n 日志：
* n 日 期        版本     修改人             修改内容
* n 2013/04/16  1.0    zhangqiushui	     创建

*/
template<class T>
class CSingleton
{
public:
    static inline T* instance();

private:
    CSingleton();
    ~CSingleton();
    CSingleton(const CSingleton&){}
    CSingleton& operator= (const CSingleton&){}

private:
    static unique_ptr<T> m_instance;
    static QMutex m_mutex;//实例互斥锁
};

template<class T>
unique_ptr<T> CSingleton<T>::m_instance;

template<class T>
QMutex CSingleton<T>::m_mutex;

template<class T>
inline T* CSingleton<T>::instance()
{
    if (nullptr == m_instance.get())
    {
        m_mutex.lock();
        if (nullptr == m_instance.get())
        {
            m_instance = unique_ptr<T>(new T);
        }
        m_mutex.unlock();
    }

    return m_instance.get();
}

// 需要使用单件模式的类在其类定义头文件中，需要调用此宏
#define DECLARE_SINGLETON_CLASS(type) \
    friend class unique_ptr<type>;\
    friend class CSingleton<type>;

#endif
