This is a course project, aim to develop a simple database acts like mysql, but with just a few functions, like 'insert', 'delete','update', no join, fix table size.

We seperate this project into 6 modules:   

-interpreter  
  deal with user input(sql), turn into internal structure  
-api  
  connect interpreter and other modules  
-record  
  organize records in file  
-catalog  
  data dictionary  
-index  
  B+ tree index  
-buffer  
  buffer that make system faster, if that block in buffer, we don't need IO operation  
  
Let's have fun!  
