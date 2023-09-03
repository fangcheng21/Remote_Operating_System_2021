function [data1]= transform(data)
recv1 = dec2hex(data);%ACSII码转换成16进制

recv2 = [];                      %将数据按小端形式放在一起
for i = 1:length(recv1)
    recv2 = [recv2,recv1(i,:)]; %小端模式
end

count = 0;
recv3 = []; %转化成double
while count < (length(recv1))/8
    recv3 = [recv3,hex2num(recv2((count*16+1):(count*16+16)))];
    count = count + 1;
end

for j=1:length(recv3)
recv4{j}=recv3(j);
end
data1 = recv4;

end