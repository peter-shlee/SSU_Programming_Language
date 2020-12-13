import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.io.File;
import java.util.ArrayList;

class ResultWindow {
    private ResultWindowFrame resultWindowFrame;
    private String pathname;
    private String keyword;

    ResultWindow(String pathname, String keyword){
        this.pathname = pathname;
        this.keyword = keyword;
        resultWindowFrame = new ResultWindowFrame();
        resultWindowFrame.setSize(700, 550);
        resultWindowFrame.setVisible(true);
    }

    private class ResultWindowFrame extends JFrame {
        JPanel resultTextPanel;
        JPanel buttonsPanel;

        JLabel pathLabel;

        JTable resultTable;
        JScrollPane scrollPane;

        JButton deleteButton;
        JButton cancelButton;

        ResultWindowFrame() throws IllegalPathnameAndKeywordException, NullPointerException, java.util.regex.PatternSyntaxException{

            resultTextPanel = new JPanel();
            buttonsPanel = new JPanel();

            pathLabel = new JLabel( "Current Location: " + pathname);

            showDirFileList(pathname, keyword);

            deleteButton = new JButton("Delete File");
            cancelButton = new JButton("Cancel");
            deleteButton.addActionListener(new ActionListener() { // 파일 삭제를 하면 현재 출력된 리스트 테이블에서도 삭제되고 동시에 디스크에서도 파일이 삭제된다.
                @Override
                synchronized public void actionPerformed(ActionEvent e)  throws IllegalPathnameAndKeywordException, NullPointerException, java.util.regex.PatternSyntaxException{ // 동시에 삭제할 파일에 접근할 수 없다.(동기화 기법 사용)
                    int row = resultTable.getSelectedRow();
                    if (row < 0) return;

                    String deleteTarget = resultTable.getValueAt(row, 2).toString();
                    File file = new File(deleteTarget);
                    if (file == null) return;
                    file.delete();
                    resultWindowFrame.remove(scrollPane);
                    showDirFileList(pathname, keyword);
                    resultWindowFrame.add("North", scrollPane);
                    SwingUtilities.updateComponentTreeUI(resultWindowFrame);
                }
            });
            cancelButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    dispatchEvent(new WindowEvent(resultWindowFrame, WindowEvent.WINDOW_CLOSING));
                }
            });

            buttonsPanel.add(deleteButton);
            buttonsPanel.add(cancelButton);
            add(pathLabel);
            add("North", scrollPane);
            add("South", buttonsPanel);
        }

        private void showDirFileList(String pathname, String keyword) throws IllegalPathnameAndKeywordException, NullPointerException, java.util.regex.PatternSyntaxException{
            DirFileList dirFileList = new DirFileList(pathname, keyword);
            ArrayList<FileInfo> dirFileArrayList = dirFileList.getDirFileList();
            String[] columnNames = dirFileList.getColumnNames();
            Object[] record = new Object[4];
            DefaultTableModel tableModel = new DefaultTableModel(columnNames, 0){
                @Override
                public boolean isCellEditable(int row, int column) {
                    return false;
                }
            };

            for(int i = 0; i < dirFileArrayList.size(); ++i){
                record[0] = dirFileArrayList.get(i).getName();
                record[1] = dirFileArrayList.get(i).getFileSize();
                record[2] = dirFileArrayList.get(i).getAbsolutePath();
                record[3] = dirFileArrayList.get(i).getLastModifiedTime();

                tableModel.addRow(record);
            }

            resultTable = new JTable(tableModel);
            resultTable.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
            resultTable.setRowHeight(20);
            scrollPane = new JScrollPane(resultTable);
        }
    }
}
