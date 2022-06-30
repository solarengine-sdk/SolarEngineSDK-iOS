//
//  SolarEngineSDK.h
//  SolarEngineSDK
//
//  Created by MVP on 2022/1/20.
//

#import <Foundation/Foundation.h>
#import "SEEventConstants.h"

NS_ASSUME_NONNULL_BEGIN

@interface SolarEngineSDK : NSObject

/// SolarEngineSDK 单例
+ (nonnull instancetype)sharedInstance;

/// 初始化 SDK
/// @param appKey  应用 appKey，请联系商务人员获取。不允许空
/// @param SEUserId 用户 ID
- (void)setAppKey:(nonnull NSString *)appKey withSEUserId:(nonnull NSString *)SEUserId;

/// 是否开启 Debug 模式（不设置时默认不开启 Debug 模式）
/// @param isDebug YES 表示开启，NO 表示关闭（上线前请设置为 NO）
- (void)setDebugModel:(BOOL)isDebug;

/// 设置自定义 URL。需在 SDK 初始化之前调用
/// @param urlString 自定义 URL
- (void)setCustomURLString:(NSString *)urlString;

/// 是否手动上报应用安装（_appInstall） 事件。需在 SDK 初始化之前调用
/// 默认为 NO，初始化 SDK 后自动上报应用安装（_appInstall） 事件。
/// 如果设置为 YES，需要主动调用 - (void)trackAppInstall 方法上报应用安装（_appInstall）事件。
/// @param isTrackInstallEventManually 是否自动上报
- (void)setIsTrackInstallEventManually:(BOOL)isTrackInstallEventManually;

#pragma 事件

/// 上报自定义事件
/// @param eventName 事件名称
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

/// 上报注册事件
/// @param attribute  SERegisterEventAttribute 实例
- (void)trackRegisterWithAttributes:(SERegisterEventAttribute *)attribute;

/// 上报登录事件
/// @param attribute  SELoginEventAttribute 实例
- (void)trackLoginWithAttributes:(SELoginEventAttribute *)attribute;

/// 上报订单事件
/// @param attribute  SEOrderEventAttribute 实例
- (void)trackOrderWithAttributes:(SEOrderEventAttribute *)attribute;

/// 开启记录时长事件（配合 - eventFinish:properties: 方法一起使用 ）
/// @param eventName 事件名
- (void)eventStart:(NSString *)eventName;

/// 结束并上报记录时长事件（配合 - eventStart: 方法一起使用 ）
/// @param eventName 事件名
/// @param properties 自定义属性
- (void)eventFinish:(NSString *)eventName properties:(NSDictionary * _Nullable )properties;

/// 手动上报应用安装（_appInstall）事件
- (void)trackAppInstall;

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
