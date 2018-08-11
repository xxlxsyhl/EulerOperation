本项目在Window 7系统下由VS 2015 professional 开发。项目功能是通过欧拉操作建立带有若干个孔的平面，然后以该平面为基面通过平移扫成生成一个三维物体，然后用openGL显示该三维物体，并且可以切换视角（按住鼠标左键拖动）和放大缩小（按住鼠标右键上下拖动）。

HalfEdge/ 是VS2015的工程文件，包括源码和测试数据。
bin/  有一个已经编译好的二进制程序和相关的运行数据
lib/  用到的第三方库freeglut.lib  64位
presentation/ 是两个在自己机器上跑出来的实例，用gif图展示

输入文件名为:data.txt

输入文件格式：
solidNum
ringNum1 ringNum2 ...
x1 y1 z1 ringNo solidNo
x2 y2 z2 ringNo solidNo
....
....
....
xn yn zn ringNo solidNo

注意：
solidNum是需要建立的solid的个数，紧接着下一行是每个solid的孔数，再下面的若干行，x y z表示坐标点，后面紧跟的是该点所属的孔的编号和所属的solid的编号。注意这里每个solid输入的只是构成这个面的所有顶点，且需要确保所这些顶点都在一个面上。

举例：
1
1

2  0  0  0  0
-2 0  0  0  0
0  2  0  0  0

1  0  0  1  0
-1 0  0  1  0
0  1  0  1  0

上面数据是一个大的三角形里面有一个小的三角形的孔，注意ringNo = 0表示的组成面的顶点， ringNo = 1表示面里编号为1的孔的顶点。