package explorercontextmenu.menu;

public class ProcessInfo
{
  public long processId;
  
  public String name;

  @Override
  public boolean equals(Object obj)
  {
    boolean result = false;
    
    if (obj != null)
    {
      if (ProcessInfo.class.isInstance(obj))
      {
        ProcessInfo other = ProcessInfo.class.cast(obj);
        result = (other.name.equals(this.name)) && (other.processId == this.processId);
      }
    }
    
    return result;
  }
}
