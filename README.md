# vp

## 项目背景

## 项目功能

- 登录模块 (以买家为例)
  - 首先在登录界面输入买家的 name 和 password
  - 然后点击确认按钮，到这里前台界面的操作已算完成
  - 紧接着，服务器根据买家提交表单中的 name 和 password 在服务器的数据库中进行校验
  - 如果服务器端对买家表单记录校验无误，那么服务器将根据 name 和 timeStamp 等属性加密生成一个 token，并以 cookie 形式返回给浏览器，当然到这里用户密码以在无形中被加密
  - 最后呢，通过 cgi 实现 session 功能记录用户状态
  - ps:有必要说明一下，登录成功后，服务端会根据 type 字段中的枚举常量来进行重定向买家 or 卖家页面

- 支付模块

- 退款模块

- 订单模块 (买家 or 卖家查询订单)

## 项目不足之处

## 项目优化

## 项目日记

## 文件说明

- performance/:网站压力测试信息文件目录 (.dat)
  - ab/: apache ab 压力测试信息文件目录
  - webbench/:webbench 压力测试信息文件目录
- cgi/:cgi 层相关
- html/:相关页面
  - txproj_index.html:登录页面
  - txproj_buyer.html:买家登录重定向后的页面
  - txproj_seller.html:卖家登录重定向后的页面
- common/:项目共用头文件目录
- include/:mysql 部分头文件
  - DataBaseMysql.h:mysql 操作接口的封装
  - MiddleHandle.h:中间处理组件相关接口的封装
- server/:
- test/:部分头文件以及封装类的测试 coding
- init.sql:mysql 初始化脚本

## 参考文章
