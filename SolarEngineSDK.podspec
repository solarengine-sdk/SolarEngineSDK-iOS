#
# Be sure to run `pod lib lint SolarEngineSDK-iOS.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'SolarEngineSDK'
  s.version          = '1.1.1'
  s.summary          = 'Official SolarEngineSDK for iOS.'

  s.homepage         = 'https://www.solar-engine.com'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.summary          = 'Official SolarEngineSDK for iOS.'
  s.author           = { 'solar-engine' => 'sdk@solar-engine.com' }
  s.source           = { :git => 'https://github.com/solarengine-sdk/SolarEngineSDK-iOS.git', :tag => s.version.to_s }

  s.ios.deployment_target = '9.0'
  s.vendored_frameworks = 'SolarEngineSDK/SolarEngineSDK.framework'

  s.frameworks = "Security", "CoreTelephony", "AdSupport", "SystemConfiguration"
  s.libraries = "sqlite3"
  s.requires_arc = true
  
  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
end
