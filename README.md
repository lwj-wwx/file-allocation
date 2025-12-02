#File-allocation

There are three types in which file-allocation can be done:

1.Contiguous Allocation:
In this method, a file is stored in one continuous block of disk space. It’s very fast for reading because everything is together, but it suffers from fragmentation and files can’t grow easily unless free space is available right next to them.

2. Linked Allocation:
Each file is stored as a chain of blocks located anywhere on the disk. Every block points to the next one. It’s great for file growth and avoids fragmentation, but random access becomes slow because you must follow the chain block by block.

3.Indexed Allocation:
Each file has an index block that stores all the addresses of its data blocks. This makes random access fast and allows the file to grow easily. The only downside is the extra overhead of maintaining the index block.
