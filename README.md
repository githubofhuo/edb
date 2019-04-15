# edb

自制数据库引擎

### 安装

boost 1.63

jdk 1.8(optional)

run ./build_boost.sh

run ./build.sh

### 使用

```cpp
./edb
edb>> help
```

连接：

```cpp
edb>> connect localhost:48127
```

Scheme:

```cpp
edb>> create test
```

Insert

```
# default _id
edb>> insert {"name":"hello", "age":"30"}
```

```
edb>> +OK _id = 1341409

edb>> insert {“_id”:1, "email": "test@test.com"}
# return
edb>> +OK _id=1
```

Delete

Without index, you can only delete data by _id.
	

```
edb>> delete {"_id":"1"}
# return
edb>> +OK
```

如果email是Index

```
edb>> delete {"email":"test@test.com"}
```

Query

```
edb>> query {"_id":"1"}
# return
edb>> {"id":"1","email": "test@test.com"}

edb>> query {"_id", "2"}
# return
edb>> NULL
```

### 