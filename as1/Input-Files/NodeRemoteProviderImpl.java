import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

class NodeRemoteProviderImpl extends UnicastRemoteObject implements NodeRemoteProvider
{
    private static final long serialVersionUID = 0L;

    NodeRemoteProviderImpl() throws RemoteException
    {
        super();
    }

    @Override
    public NodeRemote makeNode() throws RemoteException
    {
        return new NodeRemoteImpl();
    }
    
}