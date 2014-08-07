function [obj] = SerialReceive(serial_handle)
    start_byte=uint8('s');
    num_start_bytes=1;
    num_object_tag_bytes=2;
    num_object_size_bytes=2;
    num_header_bytes=num_start_bytes+num_object_tag_bytes+num_object_size_bytes;
    timeout=5; %sec
    obj=[];
    
    tic
    while(toc<timeout)
        if((serial_handle.BytesAvailable > num_header_bytes) && (fread(serial_handle,1) == start_byte));
            %read object tag
            object_tag_in_bytes=fread(serial_handle,num_object_tag_bytes);
            object_tag=typecast(uint8(object_tag_in_bytes),'uint16');

            %read object size
            object_size_in_bytes=fread(serial_handle,num_object_size_bytes);
            object_size=typecast(uint8(object_size_in_bytes),'uint16');

            %read object
            while((toc<timeout) & (serial_handle.BytesAvailable < double(object_size))); end; % wait for input buffer to contain all object bytes
            if (toc >timeout)
               disp('Error: did not read in enough object bytes before timeout')
               BufferAvailableBytes=serial_handle.BytesAvailable
               return
            end
            
            
            object_bytes=fread(serial_handle,double(object_size));
            object_bytes=uint8(object_bytes)

            %interpret payload
            switch(object_tag)
                case 0 %uint8
                    obj=typecast(object_bytes,'uint8');
                case 1 %int8
                    obj=typecast(object_bytes,'int8');
                case 2 %uint16
                    obj=typecast(object_bytes,'uint16');
                case 3 %int16
                    obj=typecast(object_bytes,'int16');
                case 4 %uint32
                    obj=typecast(object_bytes,'uint32');
                case 5 %int32
                    obj=typecast(object_bytes,'int32');
                case 6 %uint64
                    obj=typecast(object_bytes,'uint64'); 
                case 7 %int64
                    obj=typecast(object_bytes,'int64');
                case 8 %single
                    obj=typecast(object_bytes,'single');
                case 9 %double
                    obj=typecast(object_bytes,'double');

                % User Defined Objects
                case 10 %quaternion
                    obj=Quaternion;
                    obj.q0=typecast(object_bytes(1:4),'single');
                    obj.q1=typecast(object_bytes(5:8),'single');
                    obj.q2=typecast(object_bytes(9:12),'single');
                    obj.q3=typecast(object_bytes(13:16),'single');
            end
            return
        end
    end
end