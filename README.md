# QQMediaPlayerCopy
此项目为开源项目QQMediaPlayerCopy的二次开发，用于qt项目实践，在原有基础上完善了主窗口拖拽、播放进度条设置、音量控制等主要功能
<br/>
主要功能
<ol>
  <li>支持mp4、avi、flv等视频文件的播放</li>
  <li>支持网络流播放，例如http, rtsp, rtmp等网络流</li>
  <li>支持选择多个文件循环播放</li>
  <li>支持打开目录进行播放</li>
  <li>双击全屏播放</li>
  <li>支持倍速播放，可进行0.5~~2倍速播放</li>
</ol>
<br/>

# 开发环境
- Windows 11 x64
- Qt 6.3.0
- Visual Studio 2022
- VLC 3.0.8 SDK

# 依赖配置

## VLC SDK
本项目依赖VLC媒体播放器SDK。由于第三方库文件较大，未包含在仓库中，需要自行下载配置。

### 下载VLC SDK
1. 访问 [VLC官网](https://www.videolan.org/vlc/) 下载VLC 3.0.8 Windows 64位版本
2. 或直接下载SDK：[VLC 3.0.8 Win64](https://get.videolan.org/vlc/3.0.8/win64/)

### 配置步骤
1. 将下载的VLC解压到项目根目录，命名为 `vlc-3.0.8-win64`
2. 将VLC SDK中的 `libvlc.lib` 和 `libvlccore.lib` 复制到 `QQMediaPlayerCopy/` 目录
3. 确保项目目录结构如下：
   ```
   QQMediaPlayerCopy/
   ├── vlc-3.0.8-win64/        # VLC运行时文件
   ├── QQMediaPlayerCopy/
   │   ├── libvlc.lib          # VLC链接库
   │   ├── libvlccore.lib      # VLC核心库
   │   └── ...                 # 其他源代码文件
   └── ...
   ```

# 构建步骤
1. 安装Qt 6.3.0和Visual Studio 2022
2. 配置VLC SDK（参考上述依赖配置）
3. 使用Visual Studio打开 `QQMediaPlayerCopy.sln`
4. 选择x64平台，编译运行

# 运行说明
运行程序前，确保VLC的DLL文件在可执行文件目录或系统PATH中。

# 技术亮点
<ol>
  <li>实现自定义主窗口，实现拖拽功能和窗口拉伸功能</li>
  <li>实现播放进度条设置功能</li>
  <li>实现音量控制功能</li>
  <li>实现倍速播放功能</li>
  <li>实现网络流播放功能</li>
  <li>实现选择多个文件循环播放功能</li>
  <li>实现打开目录进行播放功能</li>
  <li>实现双击全屏播放功能</li>
</ol>

# 📚 项目文档

本项目提供完整的技术文档，帮助你快速理解和学习：

- **[文档中心](./docs/README.md)** - 所有文档的导航索引
- **[架构设计文档](./docs/架构设计文档.md)** - 系统架构、模块设计、设计模式详解
- **[CVlcKits API文档](./docs/CVlcKits_API文档.md)** - 核心类的完整API参考手册
- **[学习手册](./docs/学习手册.md)** - 14天学习计划、面试准备、核心知识点

## 文档特色

- ✅ 完整的架构设计说明（含Mermaid图表）
- ✅ 详细的API文档（参数、返回值、使用示例）
- ✅ 系统的学习路线（适合面试准备）
- ✅ 设计模式应用讲解
- ✅ 常见问题解答

# 许可证

本项目采用 MIT 许可证，详见 [LICENSE](./LICENSE) 文件。