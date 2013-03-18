/*
 * MATLAB Compiler: 4.17 (R2012a)
 * Date: Sun Mar 17 19:11:39 2013
 * Arguments: "-B" "macro_default" "-W" "java:MatlabUtil,MatlabUtil" "-T" "link:lib" "-d" 
 * "/Volumes/USP/mestrado/dissertacao/src/trunk/gsm-security/matlab/MatlabUtil/src" "-w" 
 * "enable:specified_file_mismatch" "-w" "enable:repeated_file" "-w" 
 * "enable:switch_ignored" "-w" "enable:missing_lib_sentinel" "-w" "enable:demo_license" 
 * "-v" 
 * "class{MatlabUtil:/Volumes/USP/mestrado/dissertacao/src/trunk/gsm-security/matlab/multGF2.m}" 
 */

package MatlabUtil;

import com.mathworks.toolbox.javabuilder.*;
import com.mathworks.toolbox.javabuilder.internal.*;

/**
 * <i>INTERNAL USE ONLY</i>
 */
public class MatlabUtilMCRFactory
{
   
    
    /** Component's uuid */
    private static final String sComponentId = "MatlabUtil_BD6A53F1D9BA54E837D384A702CF6B4B";
    
    /** Component name */
    private static final String sComponentName = "MatlabUtil";
    
   
    /** Pointer to default component options */
    private static final MWComponentOptions sDefaultComponentOptions = 
        new MWComponentOptions(
            MWCtfExtractLocation.EXTRACT_TO_CACHE, 
            new MWCtfClassLoaderSource(MatlabUtilMCRFactory.class)
        );
    
    
    private MatlabUtilMCRFactory()
    {
        // Never called.
    }
    
    public static MWMCR newInstance(MWComponentOptions componentOptions) throws MWException
    {
        if (null == componentOptions.getCtfSource()) {
            componentOptions = new MWComponentOptions(componentOptions);
            componentOptions.setCtfSource(sDefaultComponentOptions.getCtfSource());
        }
        return MWMCR.newInstance(
            componentOptions, 
            MatlabUtilMCRFactory.class, 
            sComponentName, 
            sComponentId,
            new int[]{7,17,0}
        );
    }
    
    public static MWMCR newInstance() throws MWException
    {
        return newInstance(sDefaultComponentOptions);
    }
}
