package rolandomagico.explorercontextmenu.java.tests;

import static org.junit.jupiter.api.Assertions.*;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.stream.StreamSupport;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import explorercontextmenu.menu.ExplorerContextMenu;

/**
 * Tests for the ExplorerContextMenu class.
  */
class ExplorerContextMenuTests
{
  /**
   * Copy the JNI libraries from the project output directory to the test environment.
   * @throws Exception
   */
  @BeforeAll
  public static void setUp() throws Exception 
  {
    final String[] libraries = new String[]
    {
      "./../ExplorerContextMenu.Java/bin/Release/x86/ExplorerContextMenu.Java.32.dll",
      "./../ExplorerContextMenu.Java/bin/Release/x64/ExplorerContextMenu.Java.64.dll",
    };
    
    final Path currentPath = Paths.get("").toAbsolutePath();
    
    final Path targetPath = currentPath.resolve("libraries");
    final File targetFile = targetPath.toFile();
    if (targetFile.exists() == false)
    {
      targetFile.mkdirs();
    }
    
    for (String library : libraries)
    {
      Path libraryPath = currentPath.resolve(library);
      File libraryFile = libraryPath.toFile();
      if (libraryFile.exists() && libraryFile.isFile())
      {
        Files.copy(libraryPath, targetPath.resolve(libraryFile.getName()),  StandardCopyOption.REPLACE_EXISTING);
      }
    }
  }
  
  /**
   * Test that an empty menu is created if no paths are passed. 
   */
  @Test
  void testEmptyPathsList()
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[0], true, new String[0]);
    assertFalse(menu.getEntries().iterator().hasNext());
  }
  
  /**
   * Test that a non empty menu is created if one file is passed.
   */
  @Test
  public void testOneFileAsPaths() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempFile() }, true, null);
    assertTrue(menu.getEntries().iterator().hasNext());
  }

  /**
   * Test that a non empty menu is created if two files are passed.
   */
  @Test
  public void testTwoFilesAsPaths() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempFile(), this.createTempFile() }, true, null);
    assertTrue(menu.getEntries().iterator().hasNext());
  }

  /**
   * Test that a non empty menu is created if one directory is passed.
   */
  @Test
  public void testOneDirectoryAsPaths() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempDirectory() }, true, null);
    assertTrue(menu.getEntries().iterator().hasNext());
  }

  /**
   * Test that a non empty menu is created if two directories are passed.
   */
  @Test
  public void testTwoDirectoriesAsPaths() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempDirectory() }, true, null);
    assertTrue(menu.getEntries().iterator().hasNext());
  }

  /**
   * Test that a non empty menu is created if two directories with different parents are passed.
   */
  @Test
  public void testTwoDirectoriesWithDifferentParentsAsPaths() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempDirectory(), Paths.get(this.createTempDirectory()).getParent().toAbsolutePath().toString() }, true, null);
    assertTrue(menu.getEntries().iterator().hasNext());
  }

  /**
   * Test if Beyond Compare "Select left file" is present with one file.
   * This tests will fail if Beyond compare is not installed.
   */
  @Test
  public void testBeyondCompareWithOneFile() throws IOException
  {
    ExplorerContextMenu menu = new ExplorerContextMenu(new String[] { this.createTempFile() }, true, null);
    assertTrue(
        StreamSupport.stream(
            menu.getEntries().spliterator(), false).anyMatch(
                entry -> entry.getText().equals("Select &Left File for Compare")));
  }

  private String createTempFile() throws IOException
  {
    Path tempPath = Files.createTempFile(null, null);
    tempPath.toFile().deleteOnExit();
    return tempPath.toAbsolutePath().toString();
  }

  private String createTempDirectory() throws IOException
  {
    Path tempPath = Files.createTempDirectory(null);
    tempPath.toFile().deleteOnExit();
    return tempPath.toAbsolutePath().toString();
  }
}
