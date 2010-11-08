var socket = __import__('Socket', null, {}) //����Socket library..   
  
var server = new socket.SocketServer('127.0.0.1', 8000) //����һ��Socket Server.   
  
server.on('listen', function(server) {  //�����������ʼ�������¼�   
   print('Event listen on server:' + server.localAddress + ', port:' + server.localPort)   
})   
server.on('clientClosed', function(client) {   
   print('Event clientClosed on server:' + client.address + ', port:' + client.port)   
})   
server.on('closed', function(socket) {   
   print('Event closed on server')   
})   
  
  
server.on('accept', function(client){   // һ���µ�client����   
   print('new client:' + client.address + ', port:' + client.port)   
   client.on('read', function(s) {   //clinet�������ݿɶ���   
       var input = s.read();   
       print('read data:' + input)   
       s.write(input)       //д���ݵ���   
   });   
   client.on('closed', function(s) {   
       print('bye bye client.')   
       //s.write('input')   
   });   
})   
    
server.listen("127.0.0.1", 8000)   //����һ���˿�, ��blocking, ֱ���������˱��رա�  
