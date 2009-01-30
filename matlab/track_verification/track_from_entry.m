function out = track_from_entry(entry, track_list)
    
    t_edge = [0, find(diff(track_list(:,end)))', size(track_list,1)];
    t_indx = find(track_list(:,1)==entry(1) &...
         track_list(:,3)==entry(3));
    if(length(t_indx)~=1)
        track_list(t_indx,:)
        t_indx = track_list(t_indx, end);
        track_list((t_edge(t_indx(1))+1):t_edge(t_indx(1)+1),[1:3])
        track_list((t_edge(t_indx(2))+1 ):t_edge(t_indx(2)+1),[1:3])
        out = 0;
        return
    end
        
    t_indx = track_list(t_indx, end);

    
    entry(1:3)
    
    out = track_list((t_edge(t_indx)+1):t_edge(t_indx+1),:);

end