#include "ThreadOne.h"

ThreadOne::ThreadOne(QObject *parent) : QThread(parent)
{

}
void ThreadOne::run()
{
    sleep(10);//模拟一个时长5s的复杂函数
    emit over();//复杂函数结束后发出信号
}
