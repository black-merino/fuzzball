package com.fuzzball.poisample;

import java.io.FileInputStream;

import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.DateUtil;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;

public class Main {
     
	static final String INPUT_DIR = "C:\\Users\\username\\Downloads\\";
	
    public static void main(String[] args) {
        try {
            
            String xlsxFileAddress = INPUT_DIR + "Sample1.xlsx";
             
            Workbook wb = WorkbookFactory.create(new FileInputStream(xlsxFileAddress));
             
            // 
            for (Sheet sheet : wb ) {
                for (Row row : sheet) {
                    for (Cell cell : row) {
                        System.out.print(getCellValue(cell));
                        System.out.print(" , ");
                    }
                    System.out.println();
                }
            }
           
            wb.close();
             
        }catch (Exception e) {
            e.printStackTrace();
        } finally {
             
        }
 
    }
         
    private static Object getCellValue(Cell cell) {
        switch (cell.getCellTypeEnum()) {
         
            case STRING:
                return cell.getRichStringCellValue().getString();
                 
            case NUMERIC:
                if (DateUtil.isCellDateFormatted(cell)) {
                    return cell.getDateCellValue();
                } else {
                    return cell.getNumericCellValue();
                }
                 
            case BOOLEAN:
                return cell.getBooleanCellValue();
 
            case FORMULA:
                return cell.getCellFormula();
 
            default:
                return null;
 
        }
    }   
}

