
|版本号| 更新时间  | 更新内容 |
|---| ------ | -------- |
| 1.0.0 | 2022-02-14 | 建立初稿 |
| 1.0.1 |2022-03-17 | 修改 visitorId 逻辑 |
| 1.0.2 |2022-05-12 | 请求 oneid 支持自定义域名；内部上报逻辑优化 |
| 1.0.3 |2022-05-18 |  变现广告展示事件新增 mediationPlatform（聚合广告标识）字段，adNetworkAppID 改为非必填|
| 1.1.0 |2022-06-08 | 增加变现广告点击事件、订单事件、注册事件和登录事件统计以及 ASA 广告归因服务|
| 1.1.1 |2022-07-11 | `_appInstall` 事件更改为自动采集，不支持自定义触发时机； 内部逻辑优化 |
| 1.1.2 |2022-08-08 | 增加 `_distinct_id` 和 `_distinct_id_type` 属性，用来标识设备 ID 和取值类型;<br>支持预定义事件增加自定义属性;<br>增加自定义属性命名检查，不支持以 “_” 开头;<br>优化 `_appInstall` 事件的 `_is_first_time` 和 `_is_first_day` 判断逻辑<br>优化 `_visitor_id` 取值逻辑<br>修复 `_is_first_time` 不准确问题 |



## 一、说明

### 1. 适用范围

SolarEngine 是一套支持高度自定义的数据分析系统，系统在数据埋点、属性设置、报表创建、看板配置等多方面都支持开发者按需求进行个性化定制。

本文档将针对 SDK 端的数据上报进行说明，帮助您进行数据接入。

SolarEngine iOS SDK 适用于 iOS 9.0 及以上的操作系统。


**隐私权政策：**<https://portal.solar-engine.com/privacyPolicy>

### 2. SDK 下载

- 最新 SDK 版本为 1.1.2
- 下载地址（请联系SolarEngine产品）



### 3、使用Cocoapods集成SDK

```
pod 'SolarEngineSDK'
```

## 二、接入流程


### 1. 申请 AppKey & userID

报送数据需要 AppKey 及 userID，负责人账号管登录热力引擎系统系统后可自行查询  

查询路径：

- AppKey 查询路径：资产管理-应用管理-16位 AppKey（即应用 ID）

![](https://s4.ax1x.com/2022/02/21/Hj62l9.png)

- userID 查询路径：账户管理-账号信息-密钥-16 位 userID

![](https://s4.ax1x.com/2022/02/21/Hj64w6.png)

### 2.添加SDK所需依赖框架

  使用 Security.framework 来存储设备标识,

  使用 CoreTelephony.framework 来获取运营商信息,

  使用 AdSupport.framework 来获取 Advertising Identifier 信息,

  使用 SystemConfiguration.framework 来检查当前网络环境,

  使用 libsqlite3.tbd 用来存储数据。
  
  使用 iAd.framework、 AdServices.framework、AppTrackingTransparency.framework 获取广告归因数据。
 
  
#### 添加方法：

在工程目录中,选择 TARGETS-->Build Phases-->Link Binary With Libraries-->+ -->选择 Security.framework、CoreTelephony.framework、AdSupport.framework、SystemConfiguration.framework、libsqlite3.tbd、 iAd.framework、AdServices.framework、AppTrackingTransparency.framework 等库文件。

注：如上动态库请全部配置，避免报错。


## 三、接入方法说明

### 1. 初始化 SDK

用于应用启动后，初始化 SDK。

```
- (void)setAppKey:(nonnull NSString *)appKey withSEUserId:(nonnull NSString *)SEUserId;
```

参数说明：

| **参数** | **类型**   | **是否必填** |  **说明**                             |
| ------- | :--------- | :----------- | :----------------------------------- |
| appKey   | NSString * | 是              | 应用 appKey，请联系商务人员获取。 |
| SEUserId      | NSString * | 是           | 用户 ID   ，请联系商务人员获取。             |

示例代码：

```
[[SolarEngineSDK sharedInstance] setAppKey:@"your_appKey" withSEUserId:@"your_userId"];
```


### 使用SDK归因微服务请仔细阅读以下信息

iOS 14.5 之后，iAd 服务需要用户 ATT 授权后才能获取归因数据，为了保证归因数据的准确度，建议开发者在用户 ATT 授权后再初始化 SolarEngineSDK。

初始化实例

```
#import <AppTrackingTransparency/AppTrackingTransparency.h>

[ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
        NSString *your_appKey = @"your_appKey"; 
        NSString *your_userId_SolarEngine = @"your_userId_SolarEngine"; 
        [[SolarEngineSDK sharedInstance] setAppKey:your_appKey withSEUserId:your_userId_SolarEngine];
}];
```


### 2.开启 Debug 模式

用于打印 Debug 阶段日志。

```
- (void)setDebugModel:(BOOL)isDebug;
```

参数说明：

| **参数** | **类型**   | **是否必填** |  **说明**                             |
| ------- | --------- | ----------- | ----------------------------- |
| isDebug   | BOOL | 是              | YES 表示开启 Debug 模式，NO 表示关闭 Debug 模式。上线前请设置为 NO。不调用该方法，不开启 Debug 模式 |        

示例代码：

```
[[SolarEngineSDK sharedInstance] setDebugModel:YES];
```

### 3.设置 Custom URL

设置自定义 URL。**需在 SDK 初始化之前调用**。

```
- (void)setCustomURLString:(NSString *)urlString;
```

参数说明：

| **参数** | **类型**   | **是否必填** |  **说明**                             |
| ------- | --------- | ----------- | ----------------------------- |
| urlString   | NSString | 是              | 自定义 URL |      

示例代码：

```
[[SolarEngineSDK sharedInstance] setCustomURLString:@"https://xxx.xxx.com"];
```

### 4.访客 ID & 账号 ID

#### 4.1 访客 ID

指用户在设备上安装了应用之后，登录状态之前该用户的唯一标识。

SDK 会用设备标识或随机生成唯一 ID 的方式作为访客 ID，该 ID 在首次生成后会被缓存，卸载重新安装应用不会改变访客 ID。

我们提供访客 ID 自定义设置的接口，如果您有自己的访客管理体系需要替换访客 ID，应在 SDK 初始化之后立即进行调用。

数据上报时仅以最后一次传入的访客 ID 为准，应避免多次调用造成多个非正常访客 ID 先后上报数据的情况。

- **设置访客 ID**

调用 `setVisitorID:` 来设置访客 ID：

```Objective-C
// 设置访客 ID 为 vid8709901241
[[SolarEngineSDK sharedInstance] setVisitorID:@"vid8709901241"];
```

**注意：  
1.该调用仅为向 SDK 传入访客 ID，不会上报用户设置事件。  
2.开发者设置的访客ID长度不能超过128个字符，否则会设置失败。**

- **获取访客 ID**

如果您需要获取当前访客 ID，可以调用 `getVisitorId` 获取：

```Objective-C
// 返回访客 ID，多实例的情况下，返回的是调用实例的访客 ID
NSString *visitorId = [[SolarEngineSDK sharedInstance] visitorID];
```

#### 4.2 账号 ID

指用户在应用中登录之后，登录账号在应用中的唯一标识。登录之前将以访客 ID 作为用户标识。

在账号 ID 设置完成后，在调用 `logout` 清除账号 ID之前，设置的账号 ID 将一直保留，并作为用户身份识别 ID。清除账号 ID 的操作请在有真实退出登录状态行为时进行，关闭 App、退至后台运行时无需调用。

数据上报时仅以最后一次传入的账号 ID 为准，应避免多次调用造成多个非正常账号 ID 先后上报数据的情况。

- **设置账号 ID**

调用 `login` 来设置用户的账号 ID：

```Objective-C
[[SolarEngineSDK sharedInstance] loginWithAccountID:@"aid25491084"];
```

**注意：  
1.该调用仅为向 SDK 传入账号 ID，不会上报用户登录事件。  
2.开发者设置的账号ID长度不能超过128个字符，否则会设置失败。**

- **获取账号 ID**

调用 `getAccountId` 来获取用户的账号 ID：

```Objective-C
NSString *accountID = [[SolarEngineSDK sharedInstance] accountID];
```

- **清除账号 ID**

调用 `logout` 来清除账号 ID：

```Objective-C
[[SolarEngineSDK sharedInstance] logout];
```

注意：该调用仅为通知 SDK 清除账号 ID，不会上报用户登出事件。

### 5.事件上报

在 SDK 初始化完成之后，您就可以调用下面方法来上报事件。

**注：所有事件上报时设置的自定义属性均不支持开发者传入 "_" 下划线开头的 key 值，SDK 会默认丢弃该条属性。**

#### 5.1 设置公共事件属性

对于一些重要的属性，譬如用户的会员等级、来源渠道等，这些属性需要设置在每个事件中，此时您可以将这些属性设置为公共事件属性。

公共事件属性指的就是每个事件都会带有的属性，您可以调用 `setSuperProperties` 来设置公共事件属性，我们推荐您在发送事件前，先设置公共事件属性。

公共事件属性的格式要求与事件属性一致。

```Objective-C
// 设置公共事件属性
NSDictionary *properties = @{@"vip_level":@(2)
                                 ,@"Channel":@"A1"};
[[SolarEngineSDK sharedInstance] setSuperProperties:properties];
```


如果您需要删除某个公共事件属性，您可以调用 `unsetSuperProperty` 清除其中一个公共事件属性；如果您想要清空所有公共事件属性，则可以调用 `clearSuperProperties`。

```Objective-C
// 清除一条公共事件属性，比如将之前设置 "Channel" 属性清除，之后的数据将不会该属性
[[SolarEngineSDK sharedInstance] unsetSuperProperty:@"Channel"];

// 清除所有公共事件属性
[[SolarEngineSDK sharedInstance] clearSuperProperties];
```

**注：**  

**1.公共事件属性将会被保存到缓存中，无需每次启动 App 时调用。**

**2.如果调用 setSuperProperties 上传了先前已设置过的公共事件属性，则会覆盖之前的属性。**

**3.如果公共事件属性和事件上报上传的某个属性的 key 重复，则该事件的属性会覆盖公共事件属性。**

**4.设置公共事件属性不支持开发者传入"_"下划线开头的key值，SDK会默认丢弃该条属性。**

#### 5.2 设置预置事件属性

热力引擎 SDK 支持开发者对启动、安装、退出三个预置事件设置自定义属性，通过这些自定义属性，开发者可以更方便的统计和分析数据。

```
- (void)setPresetEvent:(SEPresetEventType)eventType withProperties:(NSDictionary*)properties;
```

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
eventType |  预置事件枚举，具体如下：SEPresetEventTypeAppInstall(安装事件)、SEPresetEventTypeAppStart(启动事件)、SEPresetEventTypeAppEnd(退出事件)、SEPresetEventTypeAll(全部预置事件，即包含安装、启动、退出事件) | SEPresetEventType | 否
properties | 自定义属性| NSDictionary * | 是


**注：1.设置预置事件自定义属性的方法可以在热力引擎SDK初始化之前设置，这样设置的属性会适用于所有后续SDK产生的预置事件，如果在SDK初始化之后在设置，在此之前产生的预置事件会不包含这些设置的自定义属性。**

**2.预置事件设置的自定义属性不会被缓存，每次设置都会覆盖前一次的设置，多次设置同一个预置事件只有最后一次生效。**

**3.如果设置了 SEPresetEventType  的枚举为 SEPresetEventTypeAppInstall，会覆盖通过SEPresetEventTypeAppInstall、SEPresetEventTypeAppStart、SEPresetEventTypeAppEnd 这三种枚举设置的自定义属性，如果多次设置，只有最后一次生效。**

**4.给预置事件设置的自定义属性支持清空，可以调用对应的枚举清空，如下：
`[[SolarEngineSDK sharedInstance] setPresetEvent:SEPresetEventTypeAppStart withProperties:nil]`，
上述方法可以用来清空启动事件的自定义属性，安装与退出事件只需更改对应的枚举值即可。**

**5.另外，热力引擎 SDK 也支持全局清除预置事件的自定义属性，如下：
`[[SolarEngineSDK sharedInstance] setPresetEvent:SEPresetEventTypeAll withProperties:nil]`，<br>
上述方法可以清空所有预置事件的自定义属性，通过 SEPresetEventTypeAppInstall、SEPresetEventTypeAppStart、SEPresetEventTypeAppEnd 这三种枚举设置的自定义属性会被一并清空。**

**6.给预置事件设置的自定义属性不支持开发者传入"_"下划线开头的 key 值，SDK 会默认丢弃该条属性。**


调用示例代码:

```
[[SolarEngineSDK sharedInstance] setPresetEvent:SEPresetEventTypeAppStart withProperties:@{ 
	@"Key1": @"Value1",
	@"Key2": @"Value2"
}];
```


####5.3 预定义事件


#####5.3.1 上报应用内购买事件

App 内付费购买时，报送该事件，用于进行购买及内购收入分析


```
- (void)trackIAPWithAttributes:(SEIAPEventAttribute *)attribute;
```

`SEIAPEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
orderId |  本次购买由系统生成的订单 ID| NSString | 否
payAmount | 本次购买支付的金额| double | 否
currencyType | 支付的货币各类，遵循《ISO 4217国际标准》，如 CNY、USD | NSString | 否
payType | 支付方式：如 alipay、weixin、applepay、paypal 等| NSString | 否
productID | 购买商品的ID | NSString | 否
productName | 商品名称 | NSString | 否
productCount | 购买商品的数量| NSInteger | 否
payStatus | 支付状态,具体枚举值如下（传值传前面的枚举简称即可）：<br> SolarEngineIAPSuccess:成功 SolarEngineIAPFail:失败、SolarEngineIAPRestored:恢复|SolarEngineIAPStatus | 否
failReason |  支付失败的原因| NSString | 是
customProperties | 自定义属性 | NSDictionary *  | 是


**注：支付失败原因failReason参数仅在 payStatus 参数为 SolarEngineIAPFail 支付失败时才会传入，其他状态传""即可。**


调用示例代码:

```
SEIAPEventAttribute *attribute = [[SEIAPEventAttribute alloc] init];
attribute.productID = @"test_productID";
attribute.productName = @"test_productName";
attribute.productCount = 1234;
attribute.orderId = @"test_orderID";
attribute.payAmount = 123456;
attribute.currencyType = @"CNY";
attribute.payType = SEIAPEventPayTypeWeixin;
attribute.payStatus = SolarEngineIAPFail;
attribute.failReason = @"test_failReason";
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackIAPWithAttributes:attribute];
```

#####5.3.2 上报变现广告展示事件

App 内变现广告展示时，报送该事件，用于进行广告展示及变现收入分析。

```
- (void)trackAdImpressionWithAttributes:(SEAdImpressionEventAttribute *)attribute;
```

`SEAdImpressionEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
adNetworkPlatform | 变现平台，前面为应传值，后面为平台名称<br>csj：穿山甲国内版<br>pangle：穿山甲国际版<br>tencent：腾讯优量汇<br>baidu：百度百青藤<br>kuaishou：快手<br>oppo：OPPO<br>vivo：vivo<br>mi：小米<br>huawei：华为<br>applovin：Applovin<br>sigmob：Sigmob<br>mintegral：Mintegral<br>oneway：OneWay<br>vungle：Vungle<br>facebook：Facebook<br>admob：AdMob<br>unity：UnityAds<br>is：IronSource<br>adtiming：AdTiming<br>klein：游可赢<br>fyber：Fyber<br>chartboost：Chartboost<br>adcolony：Adcolony| NSString | 否
adType | 展示广告的类型，<br>1：激励视频、2：开屏、3：插屏、4：全屏视频、5：Banner、6：信息流、7：短视频信息流、8：大横幅 、9：视频贴片、10：其它| NSInteger | 否
adNetworkAppID | 变现平台的应用 ID|NSString | 是
adNetworkPlacementID | 变现平台的变现广告位 ID|NSString | 否
mediationPlatform | 聚合平台标识，没有聚合平台标识，请设置为 "custom"|NSString | 否
ecpm | 广告ECPM（广告千次展现的变现收入，0或负值表示没传），单位元|double | 否
currency | 展示收益的货币种类，遵循《ISO 4217国际标准》，如 CNY、USD|NSString | 否
rendered | 广告是否渲染成功,具体枚举值如下：<br>YES：成功、NO：失败|BOOL | 否
customProperties | 自定义属性 | NSDictionary *  | 是

**注：如果开发者不需要统计 rendered 参数，传 YES 即可。**


示例代码:

```
SEAdImpressionEventAttribute *attribute = [[SEAdImpressionEventAttribute alloc] init];
attribute.adNetworkPlatform = @"csj";
attribute.adType = 1;
attribute.adNetworkAppID = @"test_adNetworkAppID";
attribute.adNetworkPlacementID = @"test_adNetworkPlacementID";
attribute.mediationPlatform = @"custom";
attribute.currency = @"USD";
attribute.ecpm = 13.14;
attribute.rendered = YES;
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackAdImpressionWithAttributes:attribute];
```

#####5.3.3 上报变现广告点击事件

App 内变现广告点击时，报送该事件，用于进行广告点击数据分析。

```
- (void)trackAdClickWithAttributes:(SEAdClickEventAttribute *)attribute;
```

`SEAdClickEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
adNetworkPlatform | 变现平台，前面为应传值，后面为平台名称<br>csj：穿山甲国内版<br>pangle：穿山甲国际版<br>tencent：腾讯优量汇<br>baidu：百度百青藤<br>kuaishou：快手<br>oppo：OPPO<br>vivo：vivo<br>mi：小米<br>huawei：华为<br>applovin：Applovin<br>sigmob：Sigmob<br>mintegral：Mintegral<br>oneway：OneWay<br>vungle：Vungle<br>facebook：Facebook<br>admob：AdMob<br>unity：UnityAds<br>is：IronSource<br>adtiming：AdTiming<br>klein：游可赢<br>fyber：Fyber<br>chartboost：Chartboost<br>adcolony：Adcolony| NSString | 否
adType | 展示广告的类型，<br>1：激励视频、2：开屏、3：插屏、4：全屏视频、5：Banner、6：信息流、7：短视频信息流、8：大横幅 、9：视频贴片、10：其它| NSInteger | 否
adNetworkPlacementID | 变现平台的变现广告位 ID|NSString | 否
mediationPlatform | 聚合平台标识，没有聚合平台标识，请设置为 "custom"|NSString | 否
customProperties | 自定义属性 | NSDictionary *  | 是


示例代码:

```
SEAdClickEventAttribute *attribute = [[SEAdClickEventAttribute alloc] init];
attribute.adNetworkPlatform = @"csj";
attribute.adType = 1;
attribute.adNetworkPlacementID = @"广告位id";
attribute.mediationPlatform = @"custom";
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackAdClickWithAttributes:attribute];
```

#####5.3.4 上报注册事件

App 内用户注册时，报送该事件，用于进行用户注册数据分析。

```
- (void)trackRegisterWithAttributes:(SERegisterEventAttribute *)attribute;
```

`SERegisterEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
registerType | 注册类型，如 "WeChat"、"QQ" 等自定义值| NSString | 否 不超过 32 字符
registerStatus | 注册状态 如 "success"| NSString | 是
customProperties | 自定义属性 | NSDictionary *  | 是

示例代码:

```
SERegisterEventAttribute *attribute = [[SERegisterEventAttribute alloc] init];
attribute.registerType = @"WeChat";
attribute.registerStatus = @"success";
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackRegisterWithAttributes:attribute];
```

#####5.3.5 上报登录事件

App 内用户登录时，报送该事件，用于进行用户登录数据分析。

```
- (void)trackLoginWithAttributes:(SELoginEventAttribute *)attribute;
```

`SELoginEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
loginType | 登录类型，如 "WeChat"、"QQ" 等自定义值| NSString | 否 不超过 32 字符
loginStatus | 登录状态 如 "success"| NSString | 是
customProperties | 自定义属性 | NSDictionary *  | 是

示例代码:

```
SELoginEventAttribute *attribute = [[SELoginEventAttribute alloc] init];
attribute.loginType = @"WeChat";
attribute.loginStatus = @"success";
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackLoginWithAttributes:attribute];
```

#####5.3.6 上报订单事件

用于进行订单数据分析。

```
- (void)trackOrderWithAttributes:(SEOrderEventAttribute *)attribute;
```

`SEOrderEventAttribute` 类参数介绍:

参数名称 | 参数含义 | 参数类型 | 是否可以为空
:-: | :-: | :-: | :-:
orderID | 订单 ID | NSString | 否 不超过 128 字符
payAmount | 订单金额，单位：元 | double | 否 
currencyType | 展示收益的货币种类，遵循《ISO 4217国际标准》，如 CNY、USD | NSString | 否
payType | 支付方式：如 alipay、weixin、applepay、paypal 等| NSString | 是 不超过 32 字符
status | 订单状态| NSString | 是
customProperties | 自定义属性 | NSDictionary *  | 是

示例代码:

```
SEOrderEventAttribute *attribute = [[SEOrderEventAttribute alloc] init];
attribute.orderID = @"1000000";
attribute.payAmount = 3.14;
attribute.currencyType = @"CNY";
attribute.payType = @"aliPay";
attribute.status = @"success";
attribute.customProperties = @{
	@"Key1": @"Value1",
	@"Key2": @"Value2"
};
[[SolarEngineSDK sharedInstance] trackOrderWithAttributes:attribute];
```

#### 5.4 自定义事件

您可以调用 `track:withProperties:` 来上报事件，建议您根据先前梳理的文档来设置事件的属性以及发送信息的条件，此处以用户购买某商品作为范例：

```Objective-C
// 商店购买事件
NSDictionary *properties = @{ @"product_name" : @"商品名",
									@"product_num" : @(1),
                              @"cost":@(100),
									@"isFirstBuy":@(true)
								  };
[[SolarEngineSDK sharedInstance] track:@"isFirstBuy" withProperties:properties];
```

事件的名称是 `NSString` 类型，只能以字母开头，可包含数字，字母和下划线 "_"，长度最大为 40 个字符，对字母大小写不敏感。

- 事件的属性是一个 `NSDictionary` 对象，其中每个元素代表一个属性。

- Key 的值为属性的名称，为 `NSString` 类型，规定只能以字母开头，包含数字，字母和下划线 "_"，长度最大为 40 个字符，对字母大小写不敏感。

- Value 为该属性的值，支持 `NSString`、`NSNumber` 和 `NSArray`。 对于 `NSArray` 的元素，只支持字符串类型，对于其他类型都将强制转为字符串存储。


#### 5.5 时长事件

如果您需要记录某个事件的持续时长，可以调用 `eventStart:` 来开始计时，配置您想要计时的事件名称，当您结束该事件时，需要调用 `eventFinish:properties:`，它将会自动在您的事件属性中加入 `_duration` 这一属性来表示记录的时长，单位为秒。需要注意的是，同一个事件名只能有一个在计时的任务。

```Objective-C
// 用户进入商品页面，开始计时，记录的事件为 "Enter_Shop"
[[SolarEngineSDK sharedInstance] eventStart:@"Enter_Shop"];
// 设置事件属性，用户浏览的商品ID
NSDictionary *properties = @{ @"product_id" : @"A1354" };
// 上传事件，计时结束，"Enter_Shop" 这一事件中将会带有表示事件时长的属性 _duration
[[SolarEngineSDK sharedInstance] eventFinish:@"Enter_Shop" properties:properties];
```

### 6.用户属性

平台目前支持的用户属性设置接口为 `userInit`、`userUpdate`、`userAdd`、`userUnset`、`userAppend`、`userDelete`。

**注：用户属性上报时设置的自定义属性均不支持开发者传入"_"下划线开头的key值，SDK会默认丢弃该条属性。**

#### 6.1 userInit

如果您要上传的用户属性只要设置一次，则可以调用 `userInit ` 来进行设置，当该属性之前已经有值的时候，将会忽略这条信息，以设置首次付费时间来为例：

```Objective-C
[[SolarEngineSDK sharedInstance] userInit:@{ @"Name" : @"Tom", @"City" : @"Beijing", @"Age" : @(18), @"Books" : @[@"b1", @"b2"] }];
```

#### 6.2 userUpdate

对于一般的用户属性，您可以调用 `userUpdate` 来进行设置，使用该接口上传的属性将会覆盖原有的属性值，如果之前不存在该用户属性，则会新建该用户属性，类型与传入属性的类型一致，此处以设置城市为例：

```Objective-C
[[SolarEngineSDK sharedInstance] userUpdate:@{ @"City" : @"Shenzhen" }];
// 此时 "City" 为"Shenzhen"
```

#### 6.3 userAdd

当您要上传数值型的属性时，您可以调用 `userAdd` 来对该属性进行累加操作，如果该属性还未被设置，则会赋值 0 后再进行计算，可传入负值，等同于相减操作。此处以累计付费金额为例：

```Objective-C
[[SolarEngineSDK sharedInstance] userAdd:@{ @"TotalRevenue" : @(30) }];
// 此时 "TotalRevenue" 为 30
[[SolarEngineSDK sharedInstance] userAdd:@{ @"TotalRevenue" : @(648) }];
// 此时 "TotalRevenue" 为 678
```

> `userAdd` 设置的属性类型以及 Key 值的限制与 `userUpdate` 一致，但 Value 只允许 `NSNumber` 类型。

#### 6.4 userUnset

当您要清空用户的用户属性值时，您可以调用 `userUnset` 来对指定属性（字符串数组）进行清空操作，如果该属性还未在集群中被创建，则 `unset` **不会**创建该属性

```Objective-C
[[SolarEngineSDK sharedInstance] userUnset:@[@"Age"]];
```

> `userUnset` 传入的参数是用户属性的属性名，类型是字符串数组。


#### 6.5 userAppend

您可以调用 `userAppend` 对数组类型的用户属性进行追加操作。

```Objective-C
// 调用 userAppend 为用户属性 Books 追加元素。如果不存在，会新建该元素
[[SolarEngineSDK sharedInstance] userAppend:@{ @"Books" : @[ @"b3", @"b4" ] }];
```

#### 6.6 userDelete

如果您要删除某个用户，可以调用 `userDelete` 将这名用户删除，您将无法再查询该名用户的用户属性，但该用户产生的事件仍然可以被查询到。

```Objective-C
[[SolarEngineSDK sharedInstance] userDelete];
```

