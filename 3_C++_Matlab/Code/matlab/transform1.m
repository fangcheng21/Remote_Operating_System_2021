function [data1] = transform1(data)
send0=cell2mat(data);
send1=num2hex(send0);%转成16进制

send2=[];
for j = 1:length(send0)
    send2 = [send2,send1(j,:)]; %小端模式 %小端模式
end

count1 = 0;
send3 = []; 

%转化成double
while count1 < length(send0)*8
    send3 = [send3,hex2dec(send2((count1*2+1):(count1*2+2)))];
    count1 = count1 + 1;
end
data1 = send3;
end