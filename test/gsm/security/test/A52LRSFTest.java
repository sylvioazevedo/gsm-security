package gsm.security.test;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

import gsm.security.utils.A52R1;
import gsm.security.utils.A52R2;
import gsm.security.utils.A52R3;
import gsm.security.utils.A52R4;

/**
 *
 * @author sazevedo
 */
public class A52LRSFTest {
    
    public A52LRSFTest() {
    }
    
    @BeforeClass
    public static void setUpClass() {
    }
    
    @AfterClass
    public static void tearDownClass() {
    }
    
    @Before
    public void setUp() {
    }
    
    @After
    public void tearDown() {
    }
    
    
    @Test
    public void testR1() {
        
        A52R1 r1 = new A52R1();
        
        System.out.println("Teste");
    }
}
