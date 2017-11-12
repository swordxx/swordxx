@objc(SWORD) class SWORD : CDVPlugin {
  @objc(echo:)
  func echo(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

    let msg = command.arguments[0] as? String ?? ""

    if msg.characters.count > 0 {
      let toastController: UIAlertController = UIAlertController(title: "", message: msg, preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: msg)
    }

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(initSWORD:)
  func initSWORD(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)


      let toastController: UIAlertController = UIAlertController(title: "", message: "initSWORD", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "initSWORD")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(HTTPUtils_makeRequest:)
  func HTTPUtils_makeRequest(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "HTTPUtils_makeRequest", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "HTTPUtils_makeRequest")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getBookNames:)
  func SWModule_getBookNames(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getBookNames", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getBookNames")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getVerseKey:)
  func SWModule_getVerseKey(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getVerseKey", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getVerseKey")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_begin:)
  func SWModule_begin(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_begin", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_begin")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_previous:)
  func SWModule_previous(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_previous", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_previous")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_next:)
  func SWModule_next(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_next", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_next")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_popError:)
  func SWModule_popError(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_popError", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_popError")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getEntryAttribute:)
  func SWModule_getEntryAttribute(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getEntryAttribute", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getEntryAttribute")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getConfigEntry:)
  func SWModule_getConfigEntry(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getConfigEntry", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getConfigEntry")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getKeyChildren:)
  func SWModule_getKeyChildren(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getKeyChildren", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getKeyChildren")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getRenderHeader:)
  func SWModule_getRenderHeader(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getRenderHeader", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getRenderHeader")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_sendText:)
  func SWModule_sendText(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_sendText", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_sendText")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getRenderText:)
  func SWModule_getRenderText(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getRenderText", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getRenderText")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getRenderChapter:)
  func SWModule_getRenderChapter(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getRenderChapter", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getRenderChapter")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_search:)
  func SWModule_search(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_search", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_search")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_getKeyText:)
  func SWModule_getKeyText(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_getKeyText", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getKeyText")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWModule_setKeyText:)
  func SWModule_setKeyText(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWModule_setKeyText", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_setKeyText")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_getModuleByName:)
  func SWMgr_getModuleByName(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_getModuleByName", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_getModuleByName")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_addExtraConfig:)
  func SWMgr_addExtraConfig(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_addExtraConfig", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_addExtraConfig")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_setExtraConfigValue:)
  func SWMgr_setExtraConfigValue(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_setExtraConfigValue", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_setExtraConfigValue")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_getExtraConfigValue:)
  func SWMgr_getExtraConfigValue(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_getExtraConfigValue", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_getExtraConfigValue")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_getExtraConfigKeys:)
  func SWMgr_getExtraConfigKeys(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_getExtraConfigKeys", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_getExtraConfigKeys")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_getExtraConfigSections:)
  func SWMgr_getExtraConfigSections(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_getExtraConfigSections", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_getExtraConfigSections")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_uninstallModule:)
  func InstallMgr_uninstallModule(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_uninstallModule", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_uninstallModule")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_remoteInstallModule:)
  func InstallMgr_remoteInstallModule(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_remoteInstallModule", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_remoteInstallModule")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_getRemoteModInfoList:)
  func InstallMgr_getRemoteModInfoList(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_getRemoteModInfoList", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_getRemoteModInfoList")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_refreshRemoteSource:)
  func InstallMgr_refreshRemoteSource(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_refreshRemoteSource", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_refreshRemoteSource")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_getRemoteSources:)
  func InstallMgr_getRemoteSources(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_getRemoteSources", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_getRemoteSources")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_syncConfig:)
  func InstallMgr_syncConfig(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_syncConfig", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_syncConfig")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(InstallMgr_setUserDisclaimerConfirmed:)
  func InstallMgr_setUserDisclaimerConfirmed(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "InstallMgr_setUserDisclaimerConfirmed", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_setUserDisclaimerConfirmed")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
  @objc(SWMgr_getModInfoList:)
  func SWMgr_getModInfoList(command: CDVInvokedUrlCommand) {
    var pluginResult = CDVPluginResult(status: CDVCommandStatus_ERROR)

      let toastController: UIAlertController = UIAlertController(title: "", message: "SWMgr_getModInfoList", preferredStyle: .alert)
      
      self.viewController?.present(toastController, animated: true, completion: nil)

      DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
        toastController.dismiss(animated: true, completion: nil)
      }
        
      pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_getModInfoList")

    self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
/*
SWMgr_registerBibleSyncListener
SWMgr_sendBibleSyncMessage		
*/
}
