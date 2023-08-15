## Git 基础 
1. 使用 sha-1 散列，用以保证数据完整性。
2. 只添加数据。
3. 三中文件状态。
	* 已提交(committed).  
	* 已修改(modified).  
	* 已暂存(staged).  

#### 初始化git 的配置 
1. `/etc/gitconfig`     
	包含系统上每个用户和仓库通用配置(git config --system).      
2. `~/.gitconfig`或`~/.config/git/config`     
	__只针对当前用户__(git config --global)

3. `.git/config`. 
	针对当前仓库


### 基础操作 
1. 获取仓库
	* 初始化仓库 
	* 克隆已有仓库。
		会在本地创建一个`.git`文件夹，拉取远程所有的数据并放入文件夹，然后从中读取出最新版本文件    
		

2. 忽略文件`.gitignore`.   

	```
	*.[oa] 
	*~ 
	```
	* 所有空行或者以`#`开头的行都会被 git 忽略。 
	* 可以使用标准 glob。模式匹配， 它会递归应用在整个工作区中。 
	* 匹配模式可以以(`/`) 开头防止递归。 
	* 匹配模式可以以(`/`) 结尾制指定目录。 
	* 要忽略制定模式以外的文件或目录， 可以在模式前加上 (`!`) 取反。 

3. glob   
	* 使用`*`匹配零个或多个任意字符。 
	* `[abc]`匹配任何一个在方括号中的字符。 
	* `?` 只匹配一个任意字符。  
	* 使用两个星号(`**`)表示匹配任意中间目录. 
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	