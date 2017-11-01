var	argscheck = require('cordova/argscheck'),
	channel   = require('cordova/channel'),
	utils     = require('cordova/utils'),
	exec      = require('cordova/exec'),
	cordova   = require('cordova');

channel.createSticky('onSWORDReady');
// Tell cordova channel to wait on the CordovaInfoReady event
channel.waitForInitialization('onSWORDReady');


function InstallMgr() {
}

InstallMgr.prototype.setUserDisclaimerConfirmed = function(callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.setUserDisclaimerConfirmed", []
	);
	return retVal;
}

InstallMgr.prototype.syncConfig = function(callback, progressNotify) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.syncConfig", [progressNotify]
	);
	return retVal;
}


InstallMgr.prototype.getRemoteSources = function(callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.getRemoteSources", []
	);
	return retVal;
}


InstallMgr.prototype.refreshRemoteSource = function(sourceName, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.refreshRemoteSource", [sourceName]
	);
	return retVal;
}

InstallMgr.prototype.getRemoteModInfoList = function(sourceName, callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.getRemoteModInfoList", [sourceName]
	);
	return retVal;
}

// callback({ status : preStatus|update|complete, totalBytes : n, completedBytes : n, message : displayable });
InstallMgr.prototype.remoteInstallModule = function(sourceName, modName, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.remoteInstallModule", [sourceName, modName]
	);
	return retVal;
}

InstallMgr.prototype.uninstallModule = function(modName, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "InstallMgr.uninstallModule", [modName]
	);
	return retVal;
}

function SWModule(modInfo) {
	this.name           = modInfo.name;
	this.description    = modInfo.description;
	this.category       = modInfo.category;
	this.direction      = modInfo.direction;
	this.language       = modInfo.language;
	this.font           = modInfo.font;
	this.shortCopyright = modInfo.shortCopyright;
	this.shortPromo     = modInfo.shortPromo;
}

SWModule.prototype.SEARCHTYPE_REGEX     =  1;
SWModule.prototype.SEARCHTYPE_PHRASE    = -1;
SWModule.prototype.SEARCHTYPE_MULTIWORD = -2;
SWModule.prototype.SEARCHTYPE_ENTRYATTR = -3;
SWModule.prototype.SEARCHTYPE_LUCENE    = -4;

SWModule.prototype.SEARCHOPTION_ICASE   = 2;


SWModule.prototype.setKeyText = function(keyText, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.setKeyText", [this.name, keyText]
	);
	return retVal;
}

SWModule.prototype.search = function(expression, searchType, flags, scope, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.search", [this.name, expression, searchType, flags, scope]
	);
	return retVal;
}

SWModule.prototype.getKeyText = function(callback) {
	var retVal = null;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getKeyText", [this.name]
	);
	return retVal;
}

SWModule.prototype.getRenderText = function(callback) {
	var retVal = null;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getRenderText", [this.name]
	);
	return retVal;
}

SWModule.prototype.shareVerse = function(callback) {
	var retVal = null;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.sendText", [this.name]
	);
	return retVal;
}

/*
 * masterMod - SWModule to use for traversing chapter; null if self
 * 	useful for parallel display with alternate v11ns
 *
 * returns [ { verse : verseKey,  preVerse : 'text', text : 'text' }, ... }
 */
SWModule.prototype.getRenderChapter = function(masterMod, callback) {
	var retVal = null;
	if (!masterMod) masterMod = this;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getRenderChapter", [masterMod.name, this.name]
	);
	return retVal;
}

SWModule.prototype.getRenderHeader = function(callback) {
	var retVal = null;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getRenderHeader", [this.name]
	);
	return retVal;
}

SWModule.prototype.getKeyChildren = function(callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getKeyChildren", [this.name]
	);
	return retVal;
}

SWModule.prototype.getVerseKey = function(callback) {
	var retVal = {};
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getVerseKey", [this.name]
	);
	return retVal;
}

SWModule.prototype.getConfigEntry = function(key, callback) {
	var retVal = '';
	exec(callback?callback:function(m) { if (m) retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getConfigEntry", [this.name, key]
	);
	return retVal;
}


SWModule.prototype.popError = function(callback) {
	var retVal = 0;
	exec(callback?callback:function(m) { retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.popError", [this.name]
	);
	return retVal;
}

SWModule.prototype.getEntryAttribute = function(level1Key, level2Key, level3Key, isFiltered, callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getEntryAttribute", [this.name, level1Key, level2Key, level3Key, isFiltered]
	);
	return retVal;
}

SWModule.prototype.next = function(callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.next", [this.name]
	);
	return retVal;
}

SWModule.prototype.previous = function(callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.previous", [this.name]
	);
	return retVal;
}

SWModule.prototype.begin = function(callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.begin", [this.name]
	);
	return retVal;
}

SWModule.prototype.getBookNames = function(callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWModule.getBookNames", [this.name]
	);
	return retVal;
}

function SWMgr() {
}


SWMgr.prototype.getModInfoList = function(callback) {
	var retVal = [];
	exec(callback?callback:function(m) { if (m && m.length > 0) retVal = m; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.getModInfoList", []
	);
	return retVal;
}

SWMgr.prototype.getModuleByName = function(modName, callback) {
	var mod = null;
	exec(function(m) { if (m.name) mod = new SWModule(m); if (callback) callback(mod); },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.getModuleByName", [modName]
	);
	return mod;
}

SWMgr.prototype.getExtraConfigSections = function(callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.getExtraConfigSections", []
	);
	return retVal;
}

SWMgr.prototype.getExtraConfigKeys = function(section, callback) {
	var retVal = [];
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.getExtraConfigKeys", [section]
	);
	return retVal;
}

SWMgr.prototype.getExtraConfigValue = function(section, key, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.getExtraConfigValue", [section, key]
	);
	return retVal;
}

SWMgr.prototype.setExtraConfigValue = function(section, key, value, callback) {
	exec(callback?callback:function() {},
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.setExtraConfigValue", [section, key, value]
	);
}

SWMgr.prototype.addExtraConfig = function(confBlob, callback) {
	var retVal = null;
	exec(callback?callback:function(r) { retVal = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.addExtraConfig", [confBlob]
	);
	return retVal;
}

SWMgr.prototype.registerBibleSyncListener = function(callback) {
	exec(callback,
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.registerBibleSyncListener", []
	);
}

SWMgr.prototype.sendBibleSyncMessage = function(osisRef, callback) {
	exec(callback?callback:function() {},
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "SWMgr.sendBibleSyncMessage", [osisRef]
	);
}

function HTTPUtils() {}

HTTPUtils.prototype.METHOD_GET  =  0;
HTTPUtils.prototype.METHOD_POST =  1;

HTTPUtils.prototype.makeRequest = function(url, postData, callback, method) {
	var result = null;
	if (!method) method = this.METHOD_POST;
	exec(callback?callback:function(r) { if (r && r.length > 0) result = r; },
		function(err) { utils.alert('[ERROR] problem: ' + err); },
		"SWORD", "HTTPUtils.makeRequest", [url, postData, method]
	);
	return result;
}



/*
	public native String      getPrefixPath();
	public native String      getConfigPath();
	public native void        setGlobalOption(String option, String value);
	public native String      getGlobalOption(String option);
	public native String      getGlobalOptionTip(String option);
	public native String      filterText(String filterName, String text);
	public native String[]    getGlobalOptions();
	public native String[]    getGlobalOptionValues(String option);
	public native void        setCipherKey(String modName, String key);
	public native void        setJavascript(boolean val);
	public native String[]    getAvailableLocales();
	public native void        setDefaultLocale(String name);
*/


/**
 * This is the SWORD namespace and access to singleton SWMgr, InstallMgr, and HTTPUtils.
 * @constructor
 */
function SWORD() {

	var me = this;

	this.available = false;
	this.version = null;

	this.installMgr = new InstallMgr();
	this.mgr        = new SWMgr();
	this.httpUtils  = new HTTPUtils();

	channel.onCordovaReady.subscribe(function() {
		me.init(function(info) {
			me.available = true;
			me.version = info.version;
			channel.onSWORDReady.fire();
		},function(e) {
			me.available = false;
			utils.alert("[ERROR] Error initializing SWORD: " + e);
		});
	});
}


SWORD.prototype.CATEGORY_BIBLES       = "Biblical Texts";
SWORD.prototype.CATEGORY_COMMENTARIES = "Commentaries";
SWORD.prototype.CATEGORY_LEXDICTS     = "Lexicons / Dictionaries";
SWORD.prototype.CATEGORY_GENBOOKS     = "Generic Books";
SWORD.prototype.CATEGORY_DAILYDEVOS   = "Daily Devotional";


SWORD.prototype.init = function(successCallback, errorCallback) {
	exec(successCallback, errorCallback, "SWORD", "init", []);
};


module.exports = new SWORD();


