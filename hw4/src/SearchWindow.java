import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
class SearchWindow {
    private SearchWindowFrame searchWindowFrame;

    SearchWindow(){
        searchWindowFrame = new SearchWindowFrame();
        searchWindowFrame.setSize(500, 150);
        searchWindowFrame.setVisible(true);
        searchWindowFrame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }
    private class SearchWindowFrame extends JFrame {
        JPanel pathNamePanel, searchWordPanel, buttonsPanel;
        JLabel pathNameLabel;
        JLabel searchWordLabel;

        JTextField pathNameTextField;
        JTextField searchWordTextField;

        JButton searchButton;
        JButton exitButton;
        private SearchWindowFrame(){
            super("File Search");

            pathNamePanel = new JPanel();
            searchWordPanel = new JPanel();
            buttonsPanel = new JPanel();

            pathNameLabel = new JLabel("    Path Name :", JLabel.CENTER);
            searchWordLabel = new JLabel("Search Word :", JLabel.CENTER);

            pathNameTextField = new JTextField(30);
            searchWordTextField = new JTextField(30);

            searchButton = new JButton("Search");
            exitButton = new JButton("Exit");
            searchButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    String inputPathName = searchWindowFrame.pathNameTextField.getText();
                    String inputKeyword = searchWindowFrame.searchWordTextField.getText();

                    Thread thread = new Thread(){
                        @Override
                        public void run() { // 검색 기능은 동시에 여러 개 수행 시킬 수 있으며 각각 독립적으로 작동한다. (쓰레드 기능 사용)
                            try{
                                ResultWindow resultWindow = new ResultWindow(inputPathName, inputKeyword);
                            } catch (Exception e){
                                JOptionPane.showMessageDialog(null, e.getMessage(), "Error", JOptionPane.WARNING_MESSAGE);
                            }
                        }
                    };
                    thread.start();

                }
            });
            exitButton.addActionListener(new ActionListener() { // 프로그램 종료 버튼을 누르면 현재 실행되고 있는 모든 수행을 종료 하고 프로그램이 종료 된다.
                @Override
                public void actionPerformed(ActionEvent e) {
                    dispatchEvent(new WindowEvent(searchWindowFrame, WindowEvent.WINDOW_CLOSING));
                }
            });

            pathNamePanel.add(pathNameLabel);
            pathNamePanel.add(pathNameTextField);
            searchWordPanel.add(searchWordLabel);
            searchWordPanel.add(searchWordTextField);
            buttonsPanel.add(searchButton);
            buttonsPanel.add(exitButton);

            add("North", pathNamePanel);
            add("Center", searchWordPanel);
            add("South", buttonsPanel);
        }
    }
}
