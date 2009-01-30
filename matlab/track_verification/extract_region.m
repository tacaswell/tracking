function out = extract_region(in,center,range, plane)
    
    tmp = in(in(:,3)==plane,:);
    center(1);
    center(2);
    
    tmp = tmp(tmp(:,1)<(center(1)+range) &tmp(:,1)>(center(1)-range)...
                &tmp(:,2)<(center(2)+range) &tmp(:,2)>(center(2)-range),:);
    
    out = [];
    for j = 1:size(tmp,1)
        out = [out; track_from_entry(tmp(j,:),in)];
    end
   
end