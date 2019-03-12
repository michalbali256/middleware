import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class NodeRemote
	extends UnicastRemoteObject
	implements Node
{

    private static final long serialVersionUID = 1L;
    
    // Remotely accessible objects need a constructor.
    public NodeRemote () throws RemoteException
	{
		super (); 
    }
    
    
}