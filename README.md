# CloudMusic
QML版网易云音乐

# NOTE #
本人环境问Win7+QT5.8+MINGW<br />
要想使用本项目你需要自己编译taglib库跟QtAV库，并且确保pro文件正确的引入这两个库<br />

taglib地址为：http://taglib.org/， 该库使用cmake进行编译<br />
QtAV地址为：https://github.com/wang-bin/QtAV ，需要注意的是QtAV库在windows下要多下一个依赖库<br />
详情请看：https://github.com/wang-bin/QtAV/wiki/Build-QtAV <br />
QML无边框窗口缩放可以转至本人的另一个仓库：https://github.com/lowbees/FramlessWindowHelper

# 2017/09/19更新 #
目前基本已经完成本地音乐的播放


# 2017/09/23更新 #
更新首页个性推荐布局，本项目采用https://github.com/Binaryify/NeteaseCloudMusicApi 来获取网易云api，你需要nodejs的运行环境
![](https://github.com/lowbees/images/blob/master/CloudMusic.png)

问题1：网络图片加载太慢<br />
问题2：网络图片的频繁获取导致服务器拒绝<br />
问题3：QtAV的bug，暂停的时候seek然后再播放会导致重新播放<br />

Todo：<br />
1. 网络音频的播放
2. 网络音频的下载
