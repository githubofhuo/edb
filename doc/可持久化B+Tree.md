## 可持久化B+Tree

#### 0. 初始状态

下图中, META_PAGE中分别有字段指向B+树的root节点和freelist page;

蓝色代表数据干净, 还未被修改;

B+树节点中的数字为其占用的页号, 如根节点的4表示根节点的内容存储在4页开始的一些页中;[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt0.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt0.png)

#### 1. 修改/添加

某个事务调用修改/添加操作, 修改了某个叶节点的内容;

db将改叶节点的内容copy一份出来, 缓存在内存中, 如下图红色部分;[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt1.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt1.png)

#### 2. 删除

该事务调用了删除操作, 于是影响了某个叶节点内的内容;

同理, 拷贝出一份维护在内存中;[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt2.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt2.png)

#### 3. Rebalance-Split

在事务提交时, DB会对B+树进行rebalance;

如果某个节点内容过多, B+树会对他进行分裂, 分裂后, 会影响到父节点;

假设下面的这个节点进行了分裂, 则会形成如下图:[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt3.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt3.png)

#### 4. Rebalance-Merge

B+树的另一个性质是如果某个节点内容过少, 会和兄弟节点进行合并, 这也会影响到父节点;

假设下面这个节点进行了合并, 则形成如下图:[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt4.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt4.png)

#### 5. 持久化简介

到目前为止, 所有的修改都维护在内存中, 下面对其进行持久化;

整个持久化是从下向上的;

持久化的过程中, 会为内存中维护的节点申请页用于存储, 也会释放掉被覆盖的页;

该期间会影响freelist, 其改动也是维护在内存中;

另外, 被修改的节点, 持久化时, 所使用的页号会发生改变, 所以其父节点也会受到影响, 该影响一直持续到根;

#### 6. 为B+树申请分页[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt5.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt5.png)[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt6.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt6.png)

因为中间两个节点的页号发生了改变, 所以其父亲节点的索引页需要更新, 于是他们的父亲节点也需要改动;

[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt7.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt7.png)[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt8.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt8.png)

#### 7. 为freelist申请分页

回收老freelist所使用的页, 为新freelist申请分页:[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt10.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt10.png)

#### 8. 更新META_PAGE

由于根节点和freelist的页号发生了改变, META PAGE也会被改变, 改变先缓存在内存中;[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt11.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt11.png)

#### 9. 持久化B+树节点

接下来真正的持久化B+树节点;

#### [![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt12.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt12.png)[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt13.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt13.png)10. 持久化freelist[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt14.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt14.png)

#### 11. 更新META PAGE到磁盘[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt15.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt15.png)12. 最后结果[![img](https://github.com/qw4990/blog/raw/master/database/boltDB/pics/bolt16.png)](https://github.com/qw4990/blog/blob/master/database/boltDB/pics/bolt16.png)

## 