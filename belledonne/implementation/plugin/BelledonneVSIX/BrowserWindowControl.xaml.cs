//------------------------------------------------------------------------------
// <copyright file="BrowserWindowControl.xaml.cs" company="Microsoft">
//     Copyright (c) Microsoft.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

namespace BelledonneVSIX
{
  using System.Diagnostics.CodeAnalysis;
  using System.Windows;
  using System.Windows.Controls;

  using EnvDTE;
  using EnvDTE80;
  using System.Collections.Generic;
  using System.Diagnostics;
  using System.IO;

  /// <summary>
  /// Interaction logic for BrowserWindowControl.
  /// </summary>
  public partial class BrowserWindowControl : UserControl
  {
    static HashSet<string> extensions = new HashSet<string> { ".h", ".hpp", ".c", ".cc", ".cpp" };

    /// <summary>
    /// Initializes a new instance of the <see cref="BrowserWindowControl"/> class.
    /// </summary>
    public BrowserWindowControl()
    {
      this.InitializeComponent();
    }

    // Flattens out all child items
    static public void AddChildItems(ProjectItem baseItem, List<ProjectItem> result)
    {
      string extension = Path.GetExtension(baseItem.FileNames[1]);
      if (extension.Length > 0 && extensions.Contains(extension))
      {
        result.Add(baseItem);
      }
      if (baseItem.ProjectItems != null)
      {
        foreach (ProjectItem childItem in baseItem.ProjectItems)
        {
          AddChildItems(childItem, result);
        }
      }
    }

    static public List<ProjectItem> GetItemsInProject(Project project)
    {
      List<ProjectItem> result = new List<ProjectItem>();
      foreach (ProjectItem item in project.ProjectItems)
      {
        AddChildItems(item, result);
      }
      return result;
    }

    // This isn't straightforward due to nested subprojects weirdness
    static public List<Project> GetProjectsInSolution(Solution solution)
    {
      List<Project> result = new List<Project>();
      foreach (Project project in solution.Projects)
      {
        if (project.Kind == ProjectKinds.vsProjectKindSolutionFolder)
        {
          result.AddRange(GetNestedProjects_r(project));
        }
        else
        {
          result.Add(project);
        }
      }
      return result;
    }

    // Idea courtesy of http://www.wwwlicious.com/2011/03/29/envdte-getting-all-projects-html/
    private static List<Project> GetNestedProjects_r(Project baseProject)
    {
      List<Project> result = new List<Project>();
      foreach (ProjectItem item in baseProject.ProjectItems)
      {
        Project subProject = item.SubProject;
        if (subProject != null)
        {
          // This itself is a solution folder
          if (subProject.Kind == ProjectKinds.vsProjectKindSolutionFolder)
          {
            result.AddRange(GetNestedProjects_r(subProject));
          }
          else
          {
            result.Add(subProject);
          }
        }
      }
      return result;
    }

    /// <summary>
    /// Handles click on the button by displaying a message box.
    /// </summary>
    /// <param name="sender">The event sender.</param>
    /// <param name="e">The event args.</param>
    [SuppressMessage("Microsoft.Globalization", "CA1300:SpecifyMessageBoxOptions", Justification = "Sample code")]
    [SuppressMessage("StyleCop.CSharp.NamingRules", "SA1300:ElementMustBeginWithUpperCaseLetter", Justification = "Default event handler naming pattern")]
    private void button1_Click(object sender, RoutedEventArgs e)
    {
      listView.Items.Clear();

      Stopwatch sw = new Stopwatch();
      sw.Start();

      DTE2 dte = BrowserWindowPackage._instance._dte;
      foreach (Project project in GetProjectsInSolution(dte.Solution))
      {
        foreach (ProjectItem item in GetItemsInProject(project))
        {
          // At this point everything has already been flattened out
          listView.Items.Add(item.FileNames[(short)1] + " " + item.ContainingProject.Name);
        }
      }

      sw.Stop();

      string filePath = @"..\log.txt";
      using (System.IO.StreamWriter file = new System.IO.StreamWriter(filePath))
      {
        foreach (string line in listView.Items)
        {
          file.WriteLine(line);
        }
      }

      MessageBox.Show(
          string.Format(System.Globalization.CultureInfo.CurrentUICulture, "{0} Elapsed", sw.Elapsed),
          "BrowserWindow");
    }
  }
}