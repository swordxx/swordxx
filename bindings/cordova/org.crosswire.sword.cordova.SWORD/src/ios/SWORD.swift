import MessageUI

var mySWORDPlugin:SWORD? = nil

@objc(SWORD) class SWORD : CDVPlugin, MFMessageComposeViewControllerDelegate {
	var mgr = 0;
	var installMgr = 0
	var disclaimerConfirmed = false;
    
	@objc(initSWORD:)
	func initSWORD(command: CDVInvokedUrlCommand) {
		mgr = 0
		installMgr = 0
		disclaimerConfirmed = false
		mySWORDPlugin = nil
		VERSEKEY_BOOKABBREV = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOKABBREV);
		VERSEKEY_BOOK = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOK);
		VERSEKEY_CHAPTER = Int(org_crosswire_sword_SWModule_VERSEKEY_CHAPTER);
		VERSEKEY_VERSE = Int(org_crosswire_sword_SWModule_VERSEKEY_VERSE);
		VERSEKEY_TESTAMENT = Int(org_crosswire_sword_SWModule_VERSEKEY_TESTAMENT);
		VERSEKEY_OSISREF = Int(org_crosswire_sword_SWModule_VERSEKEY_OSISREF);
		VERSEKEY_CHAPMAX = Int(org_crosswire_sword_SWModule_VERSEKEY_CHAPMAX);
		VERSEKEY_VERSEMAX = Int(org_crosswire_sword_SWModule_VERSEKEY_VERSEMAX);
		VERSEKEY_BOOKNAME = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOKNAME);
		VERSEKEY_SHORTTEXT = Int(org_crosswire_sword_SWModule_VERSEKEY_SHORTTEXT);

		initMgr()

		let libswordVersion = String(cString: org_crosswire_sword_SWMgr_version(mgr))
		debugPrint("libswordVersion: " + libswordVersion)
		self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "initSWORD; Version: " + libswordVersion), callbackId: command.callbackId)
	}


	func myToast(message: String) {
		let toastController: UIAlertController = UIAlertController(title: "", message: message, preferredStyle: .alert)
		self.viewController?.present(toastController, animated: true, completion: nil)
		DispatchQueue.main.asyncAfter(deadline: .now() + 5) {
			toastController.dismiss(animated: true, completion: nil)
		}
	}


	func initMgr() {
		if (mgr == 0) {
			let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
			mgr = org_crosswire_sword_SWMgr_newWithPath(baseDir)
debugPrint("initMgr, mgr: " + String(describing: mgr))
		}
	}


	func reinitMgr() {
		if (mgr != 0) {
			org_crosswire_sword_SWMgr_delete(mgr)
		}
		mgr = 0
		initMgr()
	}

    
    func reinitInstall() {
        if (installMgr != 0) {
            org_crosswire_sword_InstallMgr_delete(installMgr)
        }
        installMgr = 0
        initInstall()
    }
    
    func logError(message: String) {
        org_crosswire_sword_SWlog_logError(message)
    }
    func logDebug(message: String) {
        org_crosswire_sword_SWlog_logDebug(message)
    }
    func logWarning(message: String) {
        org_crosswire_sword_SWlog_logWarning(message)
    }
    func logInformation(message: String) {
        org_crosswire_sword_SWlog_logInformation(message)
    }
    func logTimedInformation(message: String) {
        org_crosswire_sword_SWlog_logTimedInformation(message)
    }

    
    func initInstall() {
    
        if (installMgr == 0) {
            logDebug(message: "initInstall: installMgr is null");
            let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
            installMgr = org_crosswire_sword_InstallMgr_new(baseDir, { (message: Optional<UnsafePointer<Int8>>, totalBytes: u_long, completedBytes: u_long) in
                let msg = String(cString: message!)
                if (msg == "update") {
                    let response = [
                        "status": "update",
                        "totalBytes": totalBytes,
                        "completedBytes": completedBytes
                        ] as [String : Any]
                    if (mySWORDPlugin != nil && mySWORDPlugin!.callbackID != "") {
                        let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response)
                        result?.setKeepCallbackAs(true)
                        mySWORDPlugin!.commandDelegate!.send(result, callbackId: mySWORDPlugin!.callbackID)
                    }
                }
                else {
                    let response = [
                        "status": "preStatus",
                        "totalBytes": totalBytes,
                        "completedBytes": completedBytes,
                        "message": msg
                        ] as [String : Any]
                    if (mySWORDPlugin != nil && mySWORDPlugin!.callbackID != "") {
                        let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response)
                        result?.setKeepCallbackAs(true)
                        mySWORDPlugin!.commandDelegate!.send(result, callbackId: mySWORDPlugin!.callbackID)
                    }
                }
            })
            if (disclaimerConfirmed) {
                org_crosswire_sword_InstallMgr_setUserDisclaimerConfirmed(installMgr)
            }
            logDebug(message: "initInstall: instantiated InstallMgr with baseDir: \(baseDir)");
        }
    }

	@objc(SWMgr_getModuleByName:)
	func SWMgr_getModuleByName(command: CDVInvokedUrlCommand) {

		initMgr();

		let modName = command.arguments[0] as? String ?? ""
        let module = org_crosswire_sword_SWMgr_getModuleByName(mgr, modName)

        if (module == 0) {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK), callbackId: command.callbackId)
            return
        }
        
        let name = org_crosswire_sword_SWModule_getName(module)
        let description = org_crosswire_sword_SWModule_getDescription(module)
        let category = org_crosswire_sword_SWModule_getCategory(module)
        let language = org_crosswire_sword_SWModule_getConfigEntry(module, "Lang")
        let direction = org_crosswire_sword_SWModule_getConfigEntry(module, "Direction")
        let font = org_crosswire_sword_SWModule_getConfigEntry(module, "Font")
        let shortCopyright = org_crosswire_sword_SWModule_getConfigEntry(module, "ShortCopyright")
        let shortPromo = org_crosswire_sword_SWModule_getConfigEntry(module, "ShortPromo")

        let response = [
            "name": name == nil ? "" : String(cString: name!),
            "description": description == nil ? "" : String(cString: description!),
            "category": category == nil ? "" : String(cString: category!),
            "language": language == nil ? "" : String(cString: language!),
            "direction": direction == nil ? "" : String(cString: direction!),
            "font": font == nil ? "" : String(cString: font!),
            "shortCopyright": shortCopyright == nil ? "" : String(cString: shortCopyright!),
            "shortPromo": shortPromo == nil ? "" : String(cString: shortPromo!)
        ]
		self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response), callbackId: command.callbackId)
	}



    @objc(SWMgr_addExtraConfig:)
    func SWMgr_addExtraConfig(command: CDVInvokedUrlCommand) {
        let blob = command.arguments[0] as? String ?? ""
        let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
        let confPath = baseDir + "/extraConfig.conf";
        let retVal = getStringArray(buffer: org_crosswire_sword_SWConfig_augmentConfig(confPath, blob))
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
    }
    
    
    @objc(SWMgr_setExtraConfigValue:)
    func SWMgr_setExtraConfigValue(command: CDVInvokedUrlCommand) {
        let section = command.arguments[0] as? String ?? ""
        let key = command.arguments[1] as? String ?? ""
        let val = command.arguments[2] as? String ?? ""
        let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
        let confPath = baseDir + "/extraConfig.conf";
        org_crosswire_sword_SWConfig_setKeyValue(confPath, section, key, val)
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_setExtraConfigValue"), callbackId: command.callbackId)
    }

    
    @objc(SWMgr_getExtraConfigValue:)
    func SWMgr_getExtraConfigValue(command: CDVInvokedUrlCommand) {
        let section = command.arguments[0] as? String ?? ""
        let key = command.arguments[1] as? String ?? ""
        let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
        let confPath = baseDir + "/extraConfig.conf";
        let retVal = String(cString:org_crosswire_sword_SWConfig_getKeyValue(confPath, section, key))
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
    }
    
    
    @objc(SWMgr_getExtraConfigKeys:)
    func SWMgr_getExtraConfigKeys(command: CDVInvokedUrlCommand) {
        let section = command.arguments[0] as? String ?? ""
        let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
        let confPath = baseDir + "/extraConfig.conf";
        let retVal = getStringArray(buffer: org_crosswire_sword_SWConfig_getSectionKeys(confPath, section))
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
    }

    
	@objc(SWMgr_getExtraConfigSections:)
	func SWMgr_getExtraConfigSections(command: CDVInvokedUrlCommand) {
		let baseDir = (FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first?.path)! + "/sword";
		let confPath = baseDir + "/extraConfig.conf";
        let retVal = getStringArray(buffer: org_crosswire_sword_SWConfig_getSections(confPath))
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
  }
    
    
    @objc(echo:)
    func echo(command: CDVInvokedUrlCommand) {
        let msg = command.arguments[0] as? String ?? ""
        myToast(message: msg)
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: msg), callbackId: command.callbackId)
    }
    
    
    @objc(HTTPUtils_makeRequest:)
    func HTTPUtils_makeRequest(command: CDVInvokedUrlCommand) {
        var url = command.arguments[0] as? String ?? ""
        let postData = command.arguments[1] as? String ?? ""
        let method = command.arguments[2] as? Int ?? 1
        
        if method == 1 {
            url += "?" + postData
        }
        var request = URLRequest(url: URL(string: url)!)
        request.httpMethod = method == 1 ? "GET" : "POST"
        if method == 2 {
            request.httpBody = postData.data(using: .utf8)
        }
        let session = URLSession.shared
        session.dataTask(with: request) {data, response, err in
            let pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: String(data: data!, encoding: String.Encoding.utf8))
            pluginResult?.setKeepCallbackAs(false)
            self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
        }.resume()

        let pluginResult = CDVPluginResult(status: CDVCommandStatus_NO_RESULT)
        pluginResult?.setKeepCallbackAs(true)
        self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
  }
    
    
    @objc(SWModule_getBookNames:)
    func SWModule_getBookNames(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        var retVal = [String]()
        org_crosswire_sword_SWModule_begin(mod)
        while (org_crosswire_sword_SWModule_popError(mod) == 0) {
            retVal.append(getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(mod))[VERSEKEY_BOOKABBREV])
            org_crosswire_sword_SWModule_setKeyText(mod, "+book")
        }
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
    }
    
    
    @objc(SWModule_getVerseKey:)
    func SWModule_getVerseKey(command: CDVInvokedUrlCommand) {
        initMgr()
        let module = getModule(command: command)
        if (module != 0) {
            let retVal = getVerseKey(keyChildren: getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(module)))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }


    @objc(SWModule_begin:)
    func SWModule_begin(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            org_crosswire_sword_SWModule_begin(mod)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_begin"), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }

    
    @objc(SWModule_previous:)
    func SWModule_previous(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            org_crosswire_sword_SWModule_previous(mod)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_previous"), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_next:)
    func SWModule_next(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            org_crosswire_sword_SWModule_next(mod)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_next"), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_popError:)
    func SWModule_popError(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let error = Int(org_crosswire_sword_SWModule_popError(mod))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: error), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_getEntryAttribute:)
    func SWModule_getEntryAttribute(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let retVal = getStringArray(buffer: org_crosswire_sword_SWModule_getEntryAttribute(mod, command.arguments[1] as? String ?? "", command.arguments[2] as? String ?? "", command.arguments[3] as? String ?? "", (command.arguments[4] as? Bool ?? false) ? 1 : 0))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_getConfigEntry:)
    func SWModule_getConfigEntry(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let val = org_crosswire_sword_SWModule_getConfigEntry(mod, command.arguments[1] as? String ?? "")
            let retVal = val == nil ? nil : String(cString: val!)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_getKeyChildren:)
    func SWModule_getKeyChildren(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let retVal = getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(mod))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWModule_getRenderHeader:)
    func SWModule_getRenderHeader(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let header = String(cString: org_crosswire_sword_SWModule_getRenderHeader(mod))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: header), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    func messageComposeViewController(_ controller: MFMessageComposeViewController, didFinishWith result: MessageComposeResult) {
        self.webView.inputViewController?.dismiss(animated: true, completion: {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK), callbackId: self.callbackID)
        })
    }

    func sendVerse(module: Int, keyText: String) {
        if MFMessageComposeViewController.canSendText() {
            let controller = MFMessageComposeViewController()
            let verseKey = getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(module))
            let modName = String(cString: org_crosswire_sword_SWModule_getName(module))
            let verseText = String(cString: org_crosswire_sword_SWModule_stripText(module))
            
            
            controller.body = verseText + " --" + verseKey[VERSEKEY_SHORTTEXT] + " (" + modName + ")"
            controller.recipients = [""]
            controller.messageComposeDelegate = self
            self.webView.inputViewController?.present(controller, animated: true, completion: nil)
        }
    }
    @objc(SWModule_sendText:)
    func SWModule_sendText(command: CDVInvokedUrlCommand) {
        initMgr()
// Switch this to use cordova social plugin
        let mod = getModule(command: command)
        if (mod != 0) {
            mySWORDPlugin = self
            callbackID = command.callbackId
            let keyText = String(cString: org_crosswire_sword_SWModule_getKeyText(mod))
            sendVerse(module: mod, keyText: keyText)
            let result = CDVPluginResult(status: CDVCommandStatus_NO_RESULT)
            result?.setKeepCallbackAs(true)
            self.commandDelegate!.send(result, callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }

    @objc(SWModule_getRenderText:)
    func SWModule_getRenderText(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let retVal = String(cString: org_crosswire_sword_SWModule_renderText(mod))
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    @objc(SWMgr_registerBibleSyncListener:)
    func SWMgr_registerBibleSyncListener(command: CDVInvokedUrlCommand) {
        let pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_registerBibleSyncListener")
        self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
    }
    
    
    @objc(SWModule_getRenderChapter:)
    func SWModule_getRenderChapter(command: CDVInvokedUrlCommand) {
        initMgr()
        
//        DispatchQueue.global().async {
            self.initMgr()
            let masterMod = self.getModule(command: command, nameArgNumber: 0)
            let mod = self.getModule(command: command, nameArgNumber: 1)
            if (masterMod != 0 && mod != 0) {
                let r = self.renderChapter(masterMod: masterMod, mod: mod)
                self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: r), callbackId: command.callbackId)
            }
//        }
/*
        var pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_getRenderChapter")
        pluginResult?.setKeepCallbackAs(true)
        self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
*/
    }

    var VERSEKEY_BOOK = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOK);
    var VERSEKEY_CHAPTER = Int(org_crosswire_sword_SWModule_VERSEKEY_CHAPTER);
    var VERSEKEY_VERSE = Int(org_crosswire_sword_SWModule_VERSEKEY_VERSE);
    var VERSEKEY_TESTAMENT = Int(org_crosswire_sword_SWModule_VERSEKEY_TESTAMENT);
    var VERSEKEY_OSISREF = Int(org_crosswire_sword_SWModule_VERSEKEY_OSISREF);
    var VERSEKEY_CHAPMAX = Int(org_crosswire_sword_SWModule_VERSEKEY_CHAPMAX);
    var VERSEKEY_VERSEMAX = Int(org_crosswire_sword_SWModule_VERSEKEY_VERSEMAX);
    var VERSEKEY_BOOKNAME = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOKNAME);
    var VERSEKEY_SHORTTEXT = Int(org_crosswire_sword_SWModule_VERSEKEY_SHORTTEXT);
    var VERSEKEY_BOOKABBREV = Int(org_crosswire_sword_SWModule_VERSEKEY_BOOKABBREV);

    func renderChapter(masterMod: Int, mod: Int) -> [[String: Any]] {
        let saveMasterKey = String(cString: org_crosswire_sword_SWModule_getKeyText(masterMod))
        let saveKey = String(cString: org_crosswire_sword_SWModule_getKeyText(mod))
        var r = [[String: Any]]()
        var currentKey = getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(masterMod))
        let book = currentKey[VERSEKEY_BOOKABBREV]
        let chapter = currentKey[VERSEKEY_CHAPTER]
        org_crosswire_sword_SWModule_setKeyText(masterMod, book + "." + chapter + ".1")
        var verseKey = getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(masterMod))
        while (org_crosswire_sword_SWModule_popError(masterMod) == 0
            && currentKey[VERSEKEY_BOOK] == verseKey[VERSEKEY_BOOK]
            && currentKey[VERSEKEY_CHAPTER] == verseKey[VERSEKEY_CHAPTER]
            ) {
                org_crosswire_sword_SWModule_setKeyText(mod, verseKey[VERSEKEY_OSISREF])
                let error = org_crosswire_sword_SWModule_popError(mod)
                var v = [String:Any]()
                if (error == 0) {
                    v["verse"] = getVerseKey(keyChildren: getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(mod)))
                    var preVerse = ""
                    for i in getStringArray(buffer: org_crosswire_sword_SWModule_getEntryAttribute(mod, "Heading", "Preverse", "", 1)) {
                        preVerse += i
                    }
                    v["preVerse"] = preVerse
                    v["text"] = String(cString: org_crosswire_sword_SWModule_renderText(mod))
                }
                else {
                    
                }
                r.append(v)
                org_crosswire_sword_SWModule_next(masterMod)
                verseKey = getStringArray(buffer: org_crosswire_sword_SWModule_getKeyChildren(masterMod))
        }
        org_crosswire_sword_SWModule_setKeyText(masterMod, saveMasterKey)
        org_crosswire_sword_SWModule_setKeyText(mod, saveKey)
        
        return r
        
    }
    
    func getVerseKey(keyChildren:[String]) -> [String:Any] {
        var retVal = [String:Any]()
        if (keyChildren.count > 9) {
            retVal["testament"]   = Int(keyChildren[VERSEKEY_TESTAMENT]);
            retVal["book"]        = Int(keyChildren[VERSEKEY_BOOK]);
            retVal["chapter"]     = Int(keyChildren[VERSEKEY_CHAPTER]);
            retVal["verse"]       = Int(keyChildren[VERSEKEY_VERSE]);
            retVal["chapterMax"]  = Int(keyChildren[VERSEKEY_CHAPMAX]);
            retVal["verseMax"]    = Int(keyChildren[VERSEKEY_VERSEMAX]);
            retVal["bookName"]    = keyChildren[VERSEKEY_BOOKNAME];
            retVal["osisRef"]     = keyChildren[VERSEKEY_OSISREF];
            retVal["shortText"]   = keyChildren[VERSEKEY_SHORTTEXT];
            retVal["bookAbbrev"]  = keyChildren[VERSEKEY_BOOKABBREV];
        }
        return retVal;
    }

    
    @objc(SWModule_search:)
    func SWModule_search(command: CDVInvokedUrlCommand) {
        initMgr()
        let mod = getModule(command: command)
        if (mod != 0) {
            let expression = command.arguments[1] as? String ?? ""
            let searchType = command.arguments[2] as? Int32 ?? 0
            let flags = command.arguments[3] as? Int ?? 0
            let scope = command.arguments.count < 5 ? nil : command.arguments[4] as? String ?? nil
            callbackID = command.callbackId
            DispatchQueue.global().async {
                mySWORDPlugin = self
                let buffer = org_crosswire_sword_SWModule_search(mod, expression, searchType, flags, scope, { (percent: Int32) in
                    let response = [
                        "status": "update",
                        "percent": percent
                        ] as [String : Any]
                    if (mySWORDPlugin != nil && mySWORDPlugin!.callbackID != "") {
                        let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response)
                        result?.setKeepCallbackAs(true)
                        mySWORDPlugin!.commandDelegate!.send(result, callbackId: mySWORDPlugin!.callbackID)
                    }
                });
                
                self.callbackID = ""
                var response = [
                    "status": "complete",
                    "percent": 100
                ] as [String : Any]

                //UnsafeMutablePointer<UnsafePointer<Int8>?>!) -> [String] {
                var b = buffer
                var count = 0
                while let i = b?.pointee {
                    if i.key == nil {
                        break
                    }
                    count = count + 1
                    b = b?.advanced(by: 1)
                }
                let searchResults = UnsafeBufferPointer<org_crosswire_sword_SearchHit>(start: buffer, count: count);
                var results = [[String:Any]]()
                for i in searchResults {
                    let sr = [
                        "key": String(cString: i.key),
                        "score": Int(i.score)
                    ] as [String : Any]
                    results.append(sr)
                }
                response["results"] = results
                
                let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response)
                result?.setKeepCallbackAs(false)
                self.commandDelegate!.send(result, callbackId: command.callbackId)
            }
            
            let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_search")
            result?.setKeepCallbackAs(true)
            self.commandDelegate!.send(result, callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR), callbackId: command.callbackId)
        }
    }
    
    
    func getModule(command: CDVInvokedUrlCommand, nameArgNumber: Int = 0) -> Int {
        initMgr()
        let modName = command.arguments[nameArgNumber] as? String ?? ""
        let module = org_crosswire_sword_SWMgr_getModuleByName(mgr, modName)
        if (module == 0) {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR, messageAs: "couldn't find module \(modName)"), callbackId: command.callbackId)
        }
        return module
    }
    
    
    @objc(SWModule_getKeyText:)
    func SWModule_getKeyText(command: CDVInvokedUrlCommand) {
        let module = getModule(command: command)
        if (module != 0) {
            let keyText = org_crosswire_sword_SWModule_getKeyText(module)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: keyText == nil ? "" : String(cString: keyText!)), callbackId: command.callbackId)
        }
    }
    
    

    @objc(SWModule_setKeyText:)
    func SWModule_setKeyText(command: CDVInvokedUrlCommand) {
        let module = getModule(command: command)
        let keyText = command.arguments[1] as? String ?? ""
        if (module != 0) {
            org_crosswire_sword_SWModule_setKeyText(module, keyText)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWModule_setKeyText"), callbackId: command.callbackId)
        }
    }
    
    
    @objc(InstallMgr_uninstallModule:)
    func InstallMgr_uninstallModule(command: CDVInvokedUrlCommand) {
        initInstall()
        initMgr()
        let retVal = org_crosswire_sword_InstallMgr_uninstallModule(installMgr, mgr, command.arguments[0] as? String ?? "")
        if (retVal == 0) {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: retVal), callbackId: command.callbackId)
        }
        else {
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_ERROR, messageAs: retVal), callbackId: command.callbackId)
        }
    }
    
    var callbackID:String = ""
    @objc(InstallMgr_remoteInstallModule:)
    func InstallMgr_remoteInstallModule(command: CDVInvokedUrlCommand) {
        initInstall()
        initMgr()
        let repo = command.arguments[0] as? String ?? ""
        let modName = command.arguments[1] as? String ?? ""
        callbackID = command.callbackId
        DispatchQueue.global().async {
            mySWORDPlugin = self
            org_crosswire_sword_InstallMgr_remoteInstallModule(self.installMgr, self.mgr, repo, modName)
            
            self.reinitMgr()
            self.callbackID = ""
            let response = [
                "status": "complete",
                "totalBytes": 0,
                "completedBytes": 0,
                "message": "Complete"
                ] as [String : Any]
            let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: response)
            result?.setKeepCallbackAs(false)
            self.commandDelegate!.send(result, callbackId: command.callbackId)
        }

        let result = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_remoteInstallModule")
        result?.setKeepCallbackAs(true)
        self.commandDelegate!.send(result, callbackId: command.callbackId)
    }

    
    @objc(InstallMgr_getRemoteModInfoList:)
    func InstallMgr_getRemoteModInfoList(command: CDVInvokedUrlCommand) {
        initInstall()
        initMgr()
        let buffer = org_crosswire_sword_InstallMgr_getRemoteModInfoList(installMgr, mgr, command.arguments[0] as? String ?? "")
        var count = 0
        var b = buffer
        while let i = b?.pointee {
            if (i.name == nil) {
                break
            }
            count = count + 1
            b = b?.advanced(by: 1)
        }
        let modInfoList = Array(UnsafeBufferPointer<org_crosswire_sword_ModInfo>(start: buffer, count: count));
        var mods = [[AnyHashable : Any]]()
        for i in modInfoList {
            let modInfo = [
                "name": String(cString: i.name),
                "description": String(cString: i.description),
                "category": String(cString: i.category),
                "language": String(cString: i.language),
                "delta": String(cString: i.delta),
                "version": String(cString: i.version)
                ] as [AnyHashable : Any]
            mods.append(modInfo)
        }
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: mods), callbackId: command.callbackId)
    }
    
    
    @objc(InstallMgr_refreshRemoteSource:)
    func InstallMgr_refreshRemoteSource(command: CDVInvokedUrlCommand) {
        initInstall()
        DispatchQueue.global().async {
            mySWORDPlugin = self
            self.callbackID = ""
            org_crosswire_sword_InstallMgr_refreshRemoteSource(self.installMgr, command.arguments[0] as? String ?? "")
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_refreshRemoteSource"), callbackId: command.callbackId)
        }
    }

    
    func getStringArray(buffer: UnsafeMutablePointer<UnsafePointer<Int8>?>!) -> [String] {
        var sources = [String]()
        var b = buffer
        while let i = b?.pointee {
            sources.append(String(cString: i))
            b = b?.advanced(by: 1)
        }
        return sources
    }
    
    
    @objc(InstallMgr_getRemoteSources:)
    func InstallMgr_getRemoteSources(command: CDVInvokedUrlCommand) {
        initInstall()
        let sources = getStringArray(buffer: org_crosswire_sword_InstallMgr_getRemoteSources(installMgr))
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: sources), callbackId: command.callbackId)
    }

    
    @objc(InstallMgr_syncConfig:)
        func InstallMgr_syncConfig(command: CDVInvokedUrlCommand) {

        initInstall()
        DispatchQueue.global().async {
            self.callbackID = ""
            mySWORDPlugin = self
            org_crosswire_sword_InstallMgr_syncConfig(self.installMgr)
            self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_syncConfig"), callbackId: command.callbackId)
        }
    }
    
    
    @objc(InstallMgr_setUserDisclaimerConfirmed:)
    func InstallMgr_setUserDisclaimerConfirmed(command: CDVInvokedUrlCommand) {
        initInstall()
        org_crosswire_sword_InstallMgr_setUserDisclaimerConfirmed(installMgr)
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "InstallMgr_setUserDisclaimerConfirmed"), callbackId: command.callbackId)
    }
    

    @objc(SWMgr_setJavascript:)
    func SWMgr_setJavascript(command: CDVInvokedUrlCommand) {
        initMgr()
        org_crosswire_sword_SWMgr_setJavascript(mgr, command.arguments[0] as? Bool ?? true ? 1 : 0)
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "SWMgr_setJavascript"), callbackId: command.callbackId)
    }
    
    @objc(SWMgr_getModInfoList:)
    func SWMgr_getModInfoList(command: CDVInvokedUrlCommand) {
        initMgr()
        let buffer = org_crosswire_sword_SWMgr_getModInfoList(mgr)
        var b = buffer
        var count = 0
        while let i = b?.pointee {
            if i.name == nil {
                break
            }
            b = b?.advanced(by: 1)
            count = count + 1
        }
        let modInfoList = Array(UnsafeBufferPointer<org_crosswire_sword_ModInfo>(start: buffer, count: count));
        
        var mods = [[AnyHashable : Any]]()
        for i in modInfoList {
            let modInfo = [
                "name": String(cString: i.name),
                "description": String(cString: i.description),
                "category": String(cString: i.category),
                "language": String(cString: i.language),
                "delta": i.delta == nil ? "" : String(cString: i.delta),
                "version": i.version == nil ? "" : String(cString: i.version)
            ] as [AnyHashable : Any]
            mods.append(modInfo)
        }
        self.commandDelegate!.send(CDVPluginResult(status: CDVCommandStatus_OK, messageAs: mods), callbackId: command.callbackId)
    }
/*
SWMgr_registerBibleSyncListener
SWMgr_sendBibleSyncMessage		
*/
}
