# PlayGwent
client code for a game like CD Projekt's gwent card game including registration, matchmaking and multiplayer game.

# How to build & run

```bash
# clone this project
cd PlayGwent
mkdir build && cd build
# Requires Qt5 is installed
cmake ..
make -j$(nproc)
cd ../bin
./PlayGwent
```

## 程序模块介绍

#### GameController

游戏的主控制器，使用了**单体**模式，负责处理用户的菜单点击响应和游戏主循环的维护。在没有任何卡牌效果的情况下，GameController自身的逻辑可以维护一个单纯的比大小的游戏循环。但是GameController本身几乎没有数据成员，只是负责几个主要模块的调度。以下是几个主要模块：

### CardMonitor基类

提供了一系列槽用来相应特定的“时刻”的操作。其中每一个子类对应**一种带技能效果**的卡牌，从而实现了高度的抽象和可装卸。任何一张卡牌都可以随时从游戏逻辑中移除，如果不监听新的时刻或者定义新的“行为”的话，也可以很方便的添加卡牌。在程序开始前每个卡牌子类要在GameController上“注册”从而监听游戏中的时刻。

实现了要求中所有卡牌的效果。

#### AbstractUI接口

对UI层面（如用户输入、动画效果）进行抽象，对于同一套游戏逻辑可以有不同的UI实现，比如此项目中就有ConsoleView的类似控制台的UI实现和GraphicsUI的GUI式实现

#### ConsoleView

用文字和富文本信息表示不断刷新的游戏界面，并且为其中的每个**事件动画**都预留了接口，为之后实现动画效果提供了便捷

#### GraphicsUI

用GUI实现的界面，使用了GraphicsView模块并且实现了很多动画效果。以下是GUI部件：

#### CardWidget

卡片的显示类，能够完整的显示卡片的血量、护甲、CD、护盾、等级等信息，支持移动、翻转、伸缩等各种动画效果。

特别需要说明的是许多UI部件比如主菜单、新建卡牌按钮等也是采用了卡片。

### CardArrayWidget

卡组的部件，是承载“一排卡”的容器，支持两种Layout：固定总宽度，卡片允许交叠/固定卡片距离，允许用滑轮左右滑动。所有游戏场景和菜单中的卡组都是CardArrayWidget

### AbstractInputBuffer和AbstractOutputBuffer

对屏幕和网络的输入输出进行抽象，从而在游戏逻辑层面上予以统一。

#### ScreenIOBuffer

对屏幕输入缓存化，使得屏幕上的鼠标输入也和网络上的输入类似，便于逻辑上的统一。

### GameAssets

游戏数据的存放位置，便于数据在GameController和CardMonitor之间进行交互，提供了很多方便的函数来帮助访问数据和辅助计算。

### NetworkManager

管理程序的网络，解析网络上传输的json，并在游戏进行时提供和屏幕输入类似的接口。



### 功能说明

django网页端可以进行用户注册，并返回服务器上的玩家排位信息。

Java客户端会读取django的sqlite数据库，处理用户的登录请求，并且用多线程的方式支持多对用户同时对战

游戏通过合理的状态表示，支持了多人观战、

使用说明：

- 部署django客户端，在网页上进行注册。
- 连接Java服务端至django的数据库，启动服务器
- 启动客户端，输入账号和密码
- 在主菜单中点击各个按钮，可以进入匹配、卡组编辑器和选择观战的游戏界面，
- 使用鼠标左键选中卡片，如果卡片在手牌且回合刚开始，可以用右键取消选中。
- 卡组编辑器支持剪贴板的导入和导出，允许用户保存多套卡组
- 服务器端使用ELO算法计算玩家的rating
- 使用了QAnimation和Qt StyleSheet实现了多种显示效果
- 使用了json保存游戏进度，支持中途加入和旁观
- 实现了所有卡牌的技能效果

### 操作说明

使用`python manage.py runserver`启动`bin/GwentRegisterServer`下的django服务器

使用`java -jar GwentServer.jar `启动`bin/GwentRegisterServer`下的java服务器

运行`bin/PlayGwent.exe`启动客户端

- 使用鼠标左键即可选中牌，移动至合适的目标之后再点击鼠标左键即可选中目标或者移动到该位置上
- 如果选中了手牌并且允许撤销，按右键可以撤销选择重新选择手牌
- 在换牌界面下，点击右键可以在换牌场景和游戏场景之间切换
- 点击各个按钮可以实现对应的功能
