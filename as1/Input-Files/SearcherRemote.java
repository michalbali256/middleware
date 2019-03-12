import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class SearcherRemote
	extends UnicastRemoteObject
	implements Searcher
{

    private static final long serialVersionUID = 1L;
    
    // Remotely accessible objects need a constructor.
    public SearcherRemote () throws RemoteException
	{
		super (); 
    }
    
    SearcherImpl impl = new SearcherImpl();
    
    @Override
    public int getDistance(Node from, Node to)
    {
        return impl.getDistance(from, to);
    }
	
    @Override
    public int getDistanceTransitive(int neighborDistance, Node from, Node to)
    {
        return impl.getDistanceTransitive(neighborDistance, from, to);
    }
    
    
}