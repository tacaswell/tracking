function out = sort_tracks(in)
   
    [junk idx] = sort(in(:,end));
    
    out = in(idx,:);
    
end