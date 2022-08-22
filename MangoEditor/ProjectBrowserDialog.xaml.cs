using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace MangoEditor
{
    /// <summary>
    /// Interaction logic for ProjectBrowserDialog.xaml
    /// </summary>
    public partial class ProjectBrowserDialog : Window
    {
        public ProjectBrowserDialog()
        {
            InitializeComponent();
        }

        private void OnToggleButtonClick(object sender, RoutedEventArgs e)
		{
            if(sender == OpenProjectButton)
			{
                if(NewProjectButton.IsChecked == true)
				{
                    ProjectBrowserStackPanel.Margin = new Thickness(0);
                    NewProjectButton.IsChecked = false;
                }

                OpenProjectButton.IsChecked = true;

			}
            else if(sender == NewProjectButton)
			{
				if(OpenProjectButton.IsChecked == true)
				{
                    ProjectBrowserStackPanel.Margin = new Thickness(-800, 0, 0, 0);
                    OpenProjectButton.IsChecked = false;
                }

				NewProjectButton.IsChecked = true;
			}
		}
    }
}
