package org.crosswire.web.i18n;

import javax.servlet.jsp.tagext.BodyTagSupport;
import javax.servlet.jsp.tagext.BodyContent;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspTagException;
//import javax.servlet.jsp.JspWriter;
import javax.servlet.http.HttpSession;
//import java.util.HashMap;
//import java.io.StringWriter;
import java.util.Locale;
import java.util.ResourceBundle;
import java.util.MissingResourceException;
import java.util.Properties;
import java.io.FileInputStream;
import java.io.FileOutputStream;


public class TranslateTag extends BodyTagSupport {
	
	private String _key=null;
	private ResourceBundle _bundle=null;
	private String _lang=null;

	/* Uses code from GNU gettext for translation */
	/* which is covered by the LGPL */
	public String doTranslate(ResourceBundle catalog, String msgid) throws JspTagException {
		String hashkey = "" + msgid.hashCode();
		try {
			if (catalog != null) {
				String result = (String)catalog.getObject(hashkey);
				if (result != null)
					return result;
			}
		} catch (MissingResourceException e) {
			// want to add the msgid to the properties file
			Properties props = new Properties();
			
			try {
				FileInputStream tbtin = new FileInputStream(pageContext.getServletContext().getRealPath("/WEB-INF/classes/tbt.properties"));
				props.load(tbtin);
				tbtin.close();
				
				props.put(hashkey, msgid);

				FileOutputStream tbtout = new FileOutputStream(pageContext.getServletContext().getRealPath("/WEB-INF/classes/tbt.properties"));
				props.store(tbtout, null);
				tbtout.close();

			}
			catch(java.io.IOException e2)
			{
				throw new JspTagException("IO Error: " + e2.getMessage());
			}
			
		}
		return msgid;
	}

	public int doAfterBody() throws JspTagException {
		BodyContent bc = getBodyContent();
		_key = bc.getString();
		HttpSession session = pageContext.getSession();
		Locale locale = (Locale) session.getAttribute("TranslateLocale");
		
		if ((_bundle == null) || (_lang != locale.toString())) {
			_lang = locale.toString();
			_bundle = ResourceBundle.getBundle("tbt", locale);
		}
		
		return 0;
	}

	public int doEndTag() throws JspException {
		try
		{
			pageContext.getOut().write(doTranslate(_bundle, _key));
		}
		catch(java.io.IOException e)
		{
			throw new JspTagException("IO Error: " + e.getMessage());
		}
		return EVAL_PAGE;
	}


}
