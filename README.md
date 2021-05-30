# CSSHP
control system for syncore

## run build.sh in linux
```
rm Makefile
rm *.o
rm moc_*.*
rm CSSHP
qmake-arm -Makefile
make
```
执行完后，目录下会出现一个CSSHP可执行程序
需要拷贝到7寸一体机上
