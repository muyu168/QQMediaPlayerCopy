# CVlcKits API 文档

## 📋 类概述

**类名**: `CVlcKits`
**继承**: `QObject`
**头文件**: `CVlcKits.h`
**功能**: libVLC 媒体播放引擎的 C++ 封装类，提供面向对象的媒体播放接口

---

## 🎯 类职责

- 封装 libVLC 的 C 接口
- 管理媒体播放器的生命周期
- 提供播放控制功能（播放、暂停、停止、跳转）
- 管理播放列表
- 发送播放状态信号给 UI 层

---

## 📦 公共接口

### 构造函数与析构函数

#### CVlcKits()
```cpp
CVlcKits();
```

**功能描述**
构造函数，初始化成员变量

**参数**
无

**返回值**
无

**使用示例**
```cpp
CVlcKits* vlcKits = new CVlcKits();
```

---

#### ~CVlcKits()
```cpp
~CVlcKits();
```

**功能描述**
析构函数，释放 libVLC 资源

**参数**
无

**返回值**
无

**注意事项**
- 会自动释放 libVLC 实例、媒体播放器、媒体对象等资源
- 确保在程序退出前调用

---

### 初始化方法

#### initVLC()
```cpp
int initVLC();
```

**功能描述**
初始化 libVLC 实例和媒体播放器

**参数**
无

**返回值**
| 返回值 | 说明 |
|--------|------|
| 0 | 初始化成功 |
| -1 | 初始化失败 |

**使用示例**
```cpp
CVlcKits vlcKits;
if (vlcKits.initVLC() == 0) {
    qDebug() << "VLC 初始化成功";
} else {
    qDebug() << "VLC 初始化失败";
}
```

**注意事项**
- 必须在使用其他功能前调用此方法
- 只需调用一次

---

### 播放控制方法

#### play(QStringList, void*)
```cpp
int play(QStringList fileList, void* hwnd);
```

**功能描述**
播放文件列表（支持多个文件循环播放）

**参数**
| 参数名 | 类型 | 说明 | 默认值 |
|--------|------|------|--------|
| fileList | QStringList | 文件路径列表 | 无 |
| hwnd | void* | 视频输出窗口句柄（Windows HWND） | 无 |

**返回值**
| 返回值 | 说明 |
|--------|------|
| 0 | 播放成功 |
| -1 | 播放失败 |

**使用示例**
```cpp
QStringList files;
files << "D:/video1.mp4" << "D:/video2.avi";

HWND hwnd = (HWND)videoWidget->winId();
if (vlcKits->play(files, hwnd) == 0) {
    qDebug() << "开始播放";
}
```

**注意事项**
- 文件路径必须是绝对路径
- hwnd 必须是有效的窗口句柄
- 会自动创建播放列表

---

#### play(QString, void*)
```cpp
int play(const QString& url, void* hwnd);
```

**功能描述**
播放单个文件或网络流

**参数**
| 参数名 | 类型 | 说明 | 默认值 |
|--------|------|------|--------|
| url | const QString& | 文件路径或网络流 URL | 无 |
| hwnd | void* | 视频输出窗口句柄 | 无 |

**返回值**
| 返回值 | 说明 |
|--------|------|
| 0 | 播放成功 |
| -1 | 播放失败 |

**使用示例**
```cpp
// 播放本地文件
vlcKits->play("D:/video.mp4", hwnd);

// 播放网络流
vlcKits->play("http://example.com/stream.m3u8", hwnd);
vlcKits->play("rtsp://example.com/live", hwnd);
```

**支持的协议**
- 本地文件: `file://` 或直接路径
- HTTP/HTTPS: `http://`, `https://`
- RTSP: `rtsp://`
- RTMP: `rtmp://`

---

#### play()
```cpp
void play();
```

**功能描述**
继续播放（从暂停状态恢复）

**参数**
无

**返回值**
无

**使用示例**
```cpp
vlcKits->play();  // 恢复播放
```

---

#### pause()
```cpp
void pause();
```

**功能描述**
暂停播放

**参数**
无

**返回值**
无

**使用示例**
```cpp
vlcKits->pause();  // 暂停播放
```

**注意事项**
- 再次调用 `play()` 可以恢复播放
- 暂停状态下可以调用 `setVideoPostion()` 跳转

---

#### stop()
```cpp
void stop();
```

**功能描述**
停止播放

**参数**
无

**返回值**
无

**使用示例**
```cpp
vlcKits->stop();  // 停止播放
```

**注意事项**
- 停止后需要重新调用 `play(url, hwnd)` 才能播放
- 会释放当前媒体资源

---

### 播放位置控制

#### setVideoPostion(int)
```cpp
void setVideoPostion(int value);
```

**功能描述**
设置播放位置（跳转）

**参数**
| 参数名 | 类型 | 说明 | 取值范围 |
|--------|------|------|----------|
| value | int | 播放位置百分比 | 0-100 |

**返回值**
无

**使用示例**
```cpp
// 跳转到 50% 位置
vlcKits->setVideoPostion(50);

// 跳转到开头
vlcKits->setVideoPostion(0);

// 跳转到结尾
vlcKits->setVideoPostion(100);
```

**注意事项**
- value 超出范围会被限制在 0-100
- 某些格式可能不支持精确跳转

---

#### getDuration()
```cpp
libvlc_time_t getDuration();
```

**功能描述**
获取当前媒体的总时长

**参数**
无

**返回值**
| 返回值类型 | 说明 |
|-----------|------|
| libvlc_time_t | 总时长（毫秒） |

**使用示例**
```cpp
libvlc_time_t duration = vlcKits->getDuration();
int seconds = duration / 1000;
int minutes = seconds / 60;
qDebug() << "总时长:" << minutes << "分" << (seconds % 60) << "秒";
```

**注意事项**
- 需要在媒体加载后才能获取准确值
- 某些流媒体可能返回 -1（未知时长）

---

### 音量控制

#### setVolumeSliderPos(int)
```cpp
void setVolumeSliderPos(int value);
```

**功能描述**
设置音量

**参数**
| 参数名 | 类型 | 说明 | 取值范围 |
|--------|------|------|----------|
| value | int | 音量值 | 0-100 |

**返回值**
无

**使用示例**
```cpp
vlcKits->setVolumeSliderPos(50);  // 设置音量为 50%
vlcKits->setVolumeSliderPos(0);   // 静音
vlcKits->setVolumeSliderPos(100); // 最大音量
```

---

### 播放速度控制

#### setPlayRate(double)
```cpp
void setPlayRate(double rate);
```

**功能描述**
设置播放速度

**参数**
| 参数名 | 类型 | 说明 | 取值范围 |
|--------|------|------|----------|
| rate | double | 播放速度倍率 | 0.25 - 4.0 |

**返回值**
无

**使用示例**
```cpp
vlcKits->setPlayRate(1.0);   // 正常速度
vlcKits->setPlayRate(0.5);   // 0.5倍速（慢放）
vlcKits->setPlayRate(1.5);   // 1.5倍速（快放）
vlcKits->setPlayRate(2.0);   // 2倍速
```

**常用速度**
| 倍速 | 说明 |
|------|------|
| 0.5 | 慢放 |
| 0.75 | 较慢 |
| 1.0 | 正常 |
| 1.25 | 较快 |
| 1.5 | 快放 |
| 2.0 | 2倍速 |

---

### 播放列表管理

#### getCurrentIndex()
```cpp
int getCurrentIndex();
```

**功能描述**
获取当前播放的文件索引

**参数**
无

**返回值**
| 返回值类型 | 说明 |
|-----------|------|
| int | 当前文件索引（从0开始） |

**使用示例**
```cpp
int index = vlcKits->getCurrentIndex();
qDebug() << "正在播放第" << (index + 1) << "个文件";
```

---

#### addCurrentIndex()
```cpp
void addCurrentIndex();
```

**功能描述**
播放下一首（索引+1）

**参数**
无

**返回值**
无

**使用示例**
```cpp
vlcKits->addCurrentIndex();  // 播放下一首
```

**注意事项**
- 如果已经是最后一首，会循环到第一首

---

#### getVecDuration()
```cpp
vector<libvlc_time_t> getVecDuration();
```

**功能描述**
获取播放列表中所有文件的时长

**参数**
无

**返回值**
| 返回值类型 | 说明 |
|-----------|------|
| vector<libvlc_time_t> | 时长数组（毫秒） |

**使用示例**
```cpp
vector<libvlc_time_t> durations = vlcKits->getVecDuration();
for (size_t i = 0; i < durations.size(); i++) {
    qDebug() << "文件" << i << "时长:" << durations[i] / 1000 << "秒";
}
```

---

### 获取播放器对象

#### media_player()
```cpp
libvlc_media_player_t* media_player();
```

**功能描述**
获取底层 libVLC 媒体播放器对象

**参数**
无

**返回值**
| 返回值类型 | 说明 |
|-----------|------|
| libvlc_media_player_t* | libVLC 播放器指针 |

**使用示例**
```cpp
libvlc_media_player_t* player = vlcKits->media_player();
// 可以直接调用 libVLC 的 API
libvlc_time_t time = libvlc_media_player_get_time(player);
```

**注意事项**
- 仅在需要使用 libVLC 原生 API 时使用
- 不建议直接操作，可能破坏封装

---

## 📡 信号（Signals）

### sig_TimeSliderPosChanged(int)
```cpp
void sig_TimeSliderPosChanged(int pos);
```

**功能描述**
播放进度改变时发出的信号

**参数**
| 参数名 | 类型 | 说明 | 取值范围 |
|--------|------|------|----------|
| pos | int | 当前播放位置百分比 | 0-100 |

**使用示例**
```cpp
connect(vlcKits, &CVlcKits::sig_TimeSliderPosChanged,
        this, [](int pos) {
    qDebug() << "播放进度:" << pos << "%";
});
```

---

### sig_VolumeSliderPosChanged(int)
```cpp
void sig_VolumeSliderPosChanged(int volume);
```

**功能描述**
音量改变时发出的信号

**参数**
| 参数名 | 类型 | 说明 | 取值范围 |
|--------|------|------|----------|
| volume | int | 当前音量 | 0-100 |

**使用示例**
```cpp
connect(vlcKits, &CVlcKits::sig_VolumeSliderPosChanged,
        this, [](int volume) {
    qDebug() << "音量:" << volume;
});
```

---

### sig_TimeTextChanged(QString)
```cpp
void sig_TimeTextChanged(const QString& timeText);
```

**功能描述**
播放时间文本改变时发出的信号

**参数**
| 参数名 | 类型 | 说明 | 格式 |
|--------|------|------|------|
| timeText | const QString& | 时间文本 | "00:00 / 00:00" |

**使用示例**
```cpp
connect(vlcKits, &CVlcKits::sig_TimeTextChanged,
        timeLabel, &QLabel::setText);
```

---

## 💡 使用示例

### 完整播放流程

```cpp
#include "CVlcKits.h"
#include <QFileDialog>

class MainWindow : public QWidget {
    Q_OBJECT

private:
    CVlcKits* m_pVlcKits;
    QWidget* m_pVideoWidget;

public:
    MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
        // 1. 创建 VLC 对象
        m_pVlcKits = new CVlcKits(this);

        // 2. 初始化 VLC
        if (m_pVlcKits->initVLC() != 0) {
            qDebug() << "VLC 初始化失败";
            return;
        }

        // 3. 创建视频显示窗口
        m_pVideoWidget = new QWidget(this);
        m_pVideoWidget->setStyleSheet("background-color: black;");

        // 4. 连接信号
        connect(m_pVlcKits, &CVlcKits::sig_TimeSliderPosChanged,
                this, &MainWindow::onProgressChanged);
    }

    void openFile() {
        // 5. 选择文件
        QString file = QFileDialog::getOpenFileName(
            this, "打开视频", "",
            "视频文件 (*.mp4 *.avi *.mkv *.flv)");

        if (file.isEmpty()) return;

        // 6. 开始播放
        HWND hwnd = (HWND)m_pVideoWidget->winId();
        m_pVlcKits->play(file, hwnd);
    }

    void onProgressChanged(int pos) {
        qDebug() << "播放进度:" << pos << "%";
    }
};
```

### 播放列表示例

```cpp
void playPlaylist() {
    QStringList files;
    files << "D:/video1.mp4"
          << "D:/video2.avi"
          << "D:/video3.mkv";

    HWND hwnd = (HWND)videoWidget->winId();
    m_pVlcKits->play(files, hwnd);

    // 获取所有文件时长
    vector<libvlc_time_t> durations = m_pVlcKits->getVecDuration();
    for (size_t i = 0; i < durations.size(); i++) {
        qDebug() << "文件" << i << ":" << durations[i] / 1000 << "秒";
    }
}

void playNext() {
    m_pVlcKits->addCurrentIndex();  // 播放下一首
}
```

### 网络流播放示例

```cpp
void playNetworkStream() {
    QString url = "http://example.com/live.m3u8";
    HWND hwnd = (HWND)videoWidget->winId();

    if (m_pVlcKits->play(url, hwnd) == 0) {
        qDebug() << "开始播放网络流";
    } else {
        qDebug() << "播放失败";
    }
}
```

---

## ⚠️ 注意事项

### 1. 线程安全
- libVLC 的大部分 API 不是线程安全的
- 建议在主线程中调用所有方法
- 如需在其他线程操作，使用 `QMetaObject::invokeMethod`

### 2. 内存管理
- CVlcKits 继承自 QObject，可以使用 Qt 的父子对象树管理
- 确保在使用完毕后正确释放资源

### 3. 错误处理
- 播放失败时会返回 -1
- 建议检查返回值并给用户提示

### 4. 性能优化
- 避免频繁调用 `getDuration()`
- 使用信号槽机制更新 UI，避免轮询

---

## 🐛 常见问题

### Q: 播放时没有画面？
**A**: 检查以下几点：
1. 确保调用了 `initVLC()`
2. 确保传入了正确的窗口句柄 `hwnd`
3. 确保视频文件格式支持
4. 检查 libVLC 库是否正确加载

### Q: 如何获取当前播放时间？
**A**: 使用 libVLC 原生 API：
```cpp
libvlc_media_player_t* player = vlcKits->media_player();
libvlc_time_t time = libvlc_media_player_get_time(player);
```

### Q: 如何判断播放是否结束？
**A**: 监听 libVLC 的事件：
```cpp
libvlc_event_manager_t* em = libvlc_media_player_event_manager(player);
libvlc_event_attach(em, libvlc_MediaPlayerEndReached, callback, this);
```

---

## 📚 相关文档

- [libVLC 官方文档](https://www.videolan.org/developers/vlc/doc/doxygen/html/)
- [Qt 信号槽机制](https://doc.qt.io/qt-6/signalsandslots.html)
- [架构设计文档](./架构设计文档.md)

---

## 📝 版本历史

| 版本 | 日期 | 更新内容 |
|------|------|----------|
| 1.0.0 | 2026-01-22 | 初始版本 |
