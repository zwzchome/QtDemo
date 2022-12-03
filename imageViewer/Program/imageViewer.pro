#-------------------------------------------------
#
# Project created by QtCreator 2018-11-08T15:52:31
#
#-------------------------------------------------
QT       += core gui widgets xml network

TARGET = ImageViewer
TEMPLATE = app

#用pri文件管理工程中的头文件和源文件的方式会更好一些
include(./imageViewer.pri)

#依赖的头文件存放的路径，写了这个cpp才能找到对应的头文件
INCLUDEPATH += ./include

#生成文件的路径，这样可以把需要访问的东西都放在那里去
DESTDIR     = ../App     #其他目标文件生成路径,使用相对路径，不管工程怎么移动都可以生效




