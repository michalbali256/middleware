import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.Map;
import java.util.Set;

public class NodeRemoteImpl extends UnicastRemoteObject implements NodeRemote
{
    private static final long serialVersionUID = 0L;

    NodeImpl impl = new NodeImpl();

    public NodeRemoteImpl() throws RemoteException
    {
        super();
    }

    @Override
    public Set<Node> getNeighbors() 
    {
        return impl.getNeighbors();
    }

    @Override
    public Map<Node, Integer> getTransitiveNeighbors(int distance) throws RemoteException
    {
        return impl.getTransitiveNeighbors(distance);
    }

    @Override
    public void addNeighbor(Node neighbor)
    {
        impl.addNeighbor(neighbor);
    }

}