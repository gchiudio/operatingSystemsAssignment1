When compiling code, IGNORE warning that gets thrown.                 
RUN                                                                     
  gcc producer.c -pthread -lrt -o producer                            
  gcc consumer.c -pthread -lrt -o consumer                                
  ./producer & ./consumer
  
If program ends prematurely, it is possible consumer ran prior to producer, which will throw shm error.
Repeat execution till successful.
