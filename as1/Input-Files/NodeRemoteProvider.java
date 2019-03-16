import java.rmi.Remote;
import java.rmi.RemoteException;

public interface NodeRemoteProvider extends Remote
{
    NodeRemote makeNode() throws RemoteException;
}