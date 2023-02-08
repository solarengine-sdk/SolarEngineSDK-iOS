//
//  SolarEngineSDK.h
//  SolarEngineSDK
//
//  Created by MVP on 2022/1/20.
//

#import <Foundation/Foundation.h>
#import <SolarEngineSDK/SEEventConstants.h>

#define SESDKVersion @"1.1.6.0"

@class UIView, UIViewController;

@interface SEConfig : NSObject

/// 是否开启 Debug 模式（不设置时默认不开启 Debug 模式）
@property (nonatomic, assign) BOOL isDebug;

/// 是否为GDPR区域，默认为不做GDPR区域限制
@property (nonatomic, assign) BOOL isGDPRArea;

///  设置自定义 URL。需在 SDK 初始化之前调用
@property(nonatomic, assign, nullable) NSString *customURL;

@end

NS_ASSUME_NONNULL_BEGIN

@interface SolarEngineSDK : NSObject

/// SolarEngineSDK 单例
+ (nonnull instancetype)sharedInstance;

/// 初始化 SDK
/// @param appKey  应用 appKey，请联系商务人员获取。不允许空
/// @param userId 用户 ID，请联系商务人员获取。不允许空
/// @param config 配置信息
- (void)startWithAppKey:(nonnull NSString *)appKey userId:(nonnull NSString *)userId config:(SEConfig *)config;

/// 是否开启 GDPR区域限制（不设置时默认不开启 GDPR区域限制）
/// @param isGDPRArea YES 表示开启，NO 表示关闭（开启后SDK将不获取IDFA、IDFV）
- (void)setGDPRArea:(BOOL)isGDPRArea;

/// 该接口已废弃，请使用startWithAppKey:userId:config: 接口的 config.isDebug 设置debugModel
//- (void)setDebugModel:(BOOL)isDebug;

/// 该接口已废弃，请使用startWithAppKey:userId:config: 接口的 config.customURL 设置自定义URL
//- (void)setCustomURLString:(NSString *)urlString;

/// 设置预置事件属性
/// @param eventType 事件类型
/// @param properties 事件属性
- (void)setPresetEvent:(SEPresetEventType)eventType withProperties:(NSDictionary*)properties;

#pragma 事件

/// 上报自定义事件
/// @param eventName 事件名称 事件名支持大小写中英文、数字、下划线，不能以下划线开头，长度不超过 40
/// @param properties 事件属性
- (void)track:(NSString *)eventName withProperties:(NSDictionary *)properties;

/// 上报应用内购买事件
/// @param attribute SEIAPEventAttribute 实例
- (void)trackIAPWithAttributes:(SEIAPEventAttribute *)attribute;

/// 上报变现广告展示事件
/// @param attribute  SEAdImpressionEventAttribute 实例
- (void)trackAdImpressionWithAttributes:(SEAdImpressionEventAttribute *)attribute;

/// 上报变现广告点击事件
/// @param attribute  SEAdClickEventAttribute 实例
- (void)trackAdClickWithAttributes:(SEAdClickEventAttribute *)attribute;

/// 上报归因事件
/// @param attribute SEAppAttrEventAttribute 实例
- (void)trackAppAttrWithAttributes:(SEAppAttrEventAttribute *)attribute;

/// 上报注册事件
/// @param attribute  SERegisterEventAttribute 实例
- (void)trackRegisterWithAttributes:(SERegisterEventAttribute *)attribute;

/// 上报登录事件
/// @param attribute  SELoginEventAttribute 实例
- (void)trackLoginWithAttributes:(SELoginEventAttribute *)attribute;

/// 上报订单事件
/// @param attribute  SEOrderEventAttribute 实例
- (void)trackOrderWithAttributes:(SEOrderEventAttribute *)attribute;

/// 上报浏览 App 页面事件
/// @param viewController 视图控制器
/// @param properties 自定义属性
- (void)trackAppViewScreen:(UIViewController *)viewController withProperties:(NSDictionary *)properties;

/// 上报元素信息事件
/// @param view 页面元素（视图、控件）
/// @param properties 自定义属性
- (void)trackAppClick:(UIView *)view withProperties:(NSDictionary *)properties;

/// 开启记录时长事件（配合 - eventFinish:properties: 方法一起使用 ）
/// @param eventName 事件名 事件名支持大小写中英文、数字、下划线，不能以下划线开头，长度不超过 40
- (void)eventStart:(NSString *)eventName;

/// 结束并上报记录时长事件（配合 - eventStart: 方法一起使用 ）
/// @param eventName 事件名 事件名支持大小写中英文、数字、下划线，不能以下划线开头，长度不超过 40
/// @param properties 自定义属性
- (void)eventFinish:(NSString *)eventName properties:(NSDictionary * _Nullable )properties;

#pragma 访客 ID

/// 设置访客 ID
/// @param visitorId 访客 ID
- (void)setVisitorID:(nonnull NSString *)visitorId;

/// 获取访客 ID
- (nullable NSString *)visitorID;

#pragma 账户 ID

/// 登录并设置账户 ID
/// @param accountId 账户 ID
- (void)loginWithAccountID:(nonnull NSString *)accountId;

/// 账户 ID
- (NSString *)accountID;

/// 退出登录并清除账户 ID
- (void)logout;

#pragma 设置公共事件属性

/// 设置公共事件属性
/// @param properties 自定义属性
- (void)setSuperProperties:(NSDictionary *)properties;

/// 清空指定的公共事件属性的值
/// @param key  公共事件属性key
- (void)unsetSuperProperty:(NSString *)key;

/// 清空公共事件属性
- (void)clearSuperProperties;

#pragma 设置用户属性

/// 用户属性初始化设置。使用本方法上传的属性如果已经存在时不修改原有属性值，如果不存在则会新建
/// @param properties 自定义属性
- (void)userInit:(NSDictionary *)properties;

/// 用户属性更新设置。使用本方法上传的属性如果已经存在时将会覆盖原有的属性值，如果不存在则会新建
/// @param properties 自定义属性
- (void)userUpdate:(NSDictionary *)properties;

/// 用户属性累加操作
/// @param properties 自定义属性（仅对数值类型的 key 进行累加操作）
- (void)userAdd:(NSDictionary *)properties;

/// 重置用户属性。对指定属性进行清空操作
/// @param keys 自定义属性 key 数组
- (void)userUnset:(NSArray<NSString *> *)keys;

/// 追加用户属性
/// @param properties 自定义属性
- (void)userAppend:(NSDictionary *)properties;

/// 删除用户
- (void)userDelete;

@end

NS_ASSUME_NONNULL_END
