function rel_date(t_Kuka,t_server)
        Tag={0};    
        jPos= getJointsPos( t_Kuka );
     	pos=getEEFPos( t_Kuka );
    	m=getEEF_Moment(t_Kuka);
    	f=getEEF_Force(t_Kuka);


 	send = [Tag,jPos,pos,m,f];
        send3 = transform1(send);

        fwrite(t_server,send3,'char');

end
