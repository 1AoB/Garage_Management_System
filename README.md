# Garage_Management_System
基于树莓派的车库管理系统

# 说明
前三天是知识点的学习,第四天是完整的项目代码,将前三天学的知识整合在一起

# 当前项目状态
已更新完毕

# 使用语言
C/C++

# 业务逻辑
当我在键盘按下1,表示有车要进入,随后进入车辆**入库流程**;
当我在键盘按下0,表示有车要进入,随后进入车辆**出库流程**;
当我在键盘按下2,表示车库暂不营业;
## 入库流程 
先进行拍照,若识别失败,则蜂鸣器响一秒,
若识别成功后,将车牌号传回系统,调用数据库的insert函数,将车牌信息,入库时间插入到数据库.
然后通过点灯的方式进行抬杆,放杆的操作:当按下
## 出库流程
先进行拍照,若识别失败,则蜂鸣器响一秒,
