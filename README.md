When compiling code, IGNORE warning that gets thrown.                 
RUN                                                                     
  gcc producer.c -pthread -lrt -o producer                            
  gcc consumer.c -pthread -lrt -o consumer                                
  ./producer & ./consumer
                                
  Screenshot is an example of how code should run. 
