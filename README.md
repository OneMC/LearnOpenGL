# LearnOpenGL
本项目对[LearnOpenGL](https://learnopengl.com/)开源教程的代码进行了重写和优化。旨在使代码更易于理解、编译，并能在不同操作系统上顺利运行。项目采用面向对象编程方式，突出每章教程的核心内容，并解决了在`Mac`平台上出现的问题。此外，本项目将所有代码整合到一个工程中，以便于章节间的对比，同时配置了完整的环境，方便新手入门，也便于探索额外的功能。

## 特点
- **无需繁琐的环境配置**：只需下载代码，即可在 Mac (Xcode) 和 Windows (Visual Studio) 上进行编译和运行；
- **统一工程**：将代码组织为一个工程，减少了章节间的冗余代码，突出了教程的核心概念；
- **Bug 修复**：修复了`Mac`平台上可能存在的小问题，确保代码在不同平台上的稳定运行；

## 代码结构
```
- mac # xcode工程目录
- windows # visualstudio工程目录
- src
    - render # 教程核心代码
        - triangle_render
        - texture_render
        - ...
    - common # 辅助代码
        - resource # 文件读取
        - model # 模型加载
        - ...
- vendor # 三方依赖库
- resources # 图片&模型资源
```

---

**注意**：本项目是基于`LearnOpenGL`教程的重写，并旨在帮助学习和教育目的。原始教程内容和代码版权归`LearnOpenGL`和相应作者所有。

原始LearnOpenGL教程:
- [英文版教程](https://learnopengl.com/)
- [中文版教程](https://learnopengl-cn.github.io/)
- [LearnOpenGL源码](https://github.com/JoeyDeVries/LearnOpenGL)
