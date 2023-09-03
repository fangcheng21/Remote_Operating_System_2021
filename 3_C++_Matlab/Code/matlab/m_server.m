close all,clear all,clc
warning('off');
ip='172.31.1.147'; % The IP of the controller
% start a connection with the server
t_Kuka=net_establishConnection( ip );

if ~exist('t_Kuka','var') || isempty(t_Kuka) || strcmp(t_Kuka.Status,'closed')
  warning('Connection could not be establised, script aborted');
  return;
end


%%%%%%%%%%%%%%%%%
t_server=tcpip('127.0.0.1',6668,'NetworkRole','server');%与第�?个请求连接的客户机建立连接，端口号为6668，类型为服务器�??
t_server.InputBuffersize=11000;%缓冲区放大到10000
fopen(t_server);%打开服务器，直到建立�?个TCP连接才返回；
sprintf("成功建立连接");
pause(1);


%%%%%%%%%%%%%%%%%%
t = timer('name','timer_1','TimerFcn','rel_date(t_Kuka,t_server);', 'Period', 0.05, 'ExecutionMode', 'fixedSpacing');



%%%%%%%%%%%%%%%%%%
%��ѭ��
%%%%%%%%%%%%%%%%%%
while(1)
    
        while(1)%等到缓存区有数据就跳出循�?
            if  t_server.BytesAvailable>0
                %t_server.BytesAvailable%显示缓存区字节数
           break;
           end
        end
        data_recv=(fread(t_server,t_server.BytesAvailable));%从缓冲区读取数字数据
        recv4 = transform(data_recv);
        

        if recv4{1}==1
             disp("moveJ");
             num = length(recv4)-1;
             for j=2:num
                data{j-1}=recv4{j};
             end
		relvel=cell2mat(recv4(length(recv4)));
		movePTPJointSpace(t_Kuka,data,relvel);
             
        elseif recv4{1}==2
             disp("moveP");
             num = length(recv4)-1;
             for j=2:num
                data1{j-1}=recv4{j};
             end
		relvel=cell2mat(recv4(length(recv4)));
		movePTPLineEEF(t_Kuka,data1,relvel);

        elseif recv4{1}==3
		disp("Start direct servo in Cartesian space");

		realTime_startDirectServoCartesian(t_Kuka);


        elseif recv4{1}==4
		disp("Start direct servo in joint space");
		
		realTime_startDirectServoJoints(t_Kuka);

        elseif recv4{1}==5
		disp("Start direct servo in velocities level");
		
		realTime_startVelControlJoints(t_Kuka);

        elseif recv4{1}==6
		disp("Stop the direct servo motion");

		realTime_stopDirectServoCartesian( t_Kuka );

        elseif recv4{1}==7
		disp("Stop the direct servo motion");

 		realTime_stopDirectServoJoints( t_Kuka );

        elseif recv4{1}==8
		disp("Stop the direct servo motion");

		realTime_stopVelControlJoints(t_Kuka);

        elseif recv4{1}==9

             for j=2:length(recv4)
                data2{j-1}=recv4{j};
             end
   	     sendJointsPositions( t_Kuka ,data2);

        elseif recv4{1}==10

             for j=2:length(recv4)
                data3{j-1}=recv4{j};
             end
   	     sendEEfPositionf( t_Kuka ,data3);

	     %% MT: is 7 cell array of doubles of the Joints measured torques
		%mt=sendEEfPositionMTorque( t_Kuka ,data); 
		%Tag = {1};
 		%send = [Tag,mt];
     	 	%send3 = transform1(send);
 	  	%fwrite(t_server,send3,'char');

        elseif recv4{1}==11

             for j=2:length(recv4)
                data4{j-1}=recv4{j};
             end
   	     sendJointsVelocities( t_Kuka ,data4);
 

        elseif recv4{1}==12
		start(t);
             
        elseif recv4{1}==13
		stop(t);
 
        elseif recv4{1}==14
		disp("getJointpos");
            Tag = {2};
        jPos= getJointsPos( t_Kuka );
        send = [Tag,jPos];
        send3 = transform1(send);
        fwrite(t_server,send3,'char');
        
        elseif recv4{1}==15
		disp("getEEFPos");
            Tag = {3};
		pos=getEEFPos( t_Kuka );
        send = [Tag,pos];
        send3 = transform1(send);
        fwrite(t_server,send3,'char');
        
        elseif recv4{1}==16
		disp("getEEF_Moment");
            Tag = {4};
		m=getEEF_Moment(t_Kuka);
        send = [Tag,m];
        send3 = transform1(send);
        fwrite(t_server,send3,'char');
        
        elseif recv4{1}==17
		disp("getEEF_Force");
            Tag = {5};
		f=getEEF_Force(t_Kuka);
        send = [Tag,f];
        send3 = transform1(send);
        fwrite(t_server,send3,'char');
        
        elseif recv4{1}==18
            disp("stop!"); 
            
                 fclose(t_server);

             delete(t);
             
        net_turnOffServer( t_Kuka );
        fclose(t_Kuka);
             
            disp("已关闭连�?");
          break;                
       end

end
