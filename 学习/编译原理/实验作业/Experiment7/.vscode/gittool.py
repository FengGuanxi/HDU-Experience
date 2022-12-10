import platform
import configparser
import pathlib
from urllib.parse import urlparse

def getPlatformURL():
    strGitconfigPath = ""
    if platform.system().lower() == 'windows':
        strGitconfigPath = ".\\.git\\config" 
    else:
        strGitconfigPath = "./.git/config"
    gitConfigPath = pathlib.Path(strGitconfigPath)
    
    # 判断是否存在.git/config文件
    if gitConfigPath.exists() and gitConfigPath.is_file():
        config = configparser.ConfigParser()
        config.read(strGitconfigPath, encoding='UTF-8')
        remoteURL = config.get('remote \"origin\"', 'url')
        res = urlparse(remoteURL)
        return res.scheme + '://' + res.netloc  
