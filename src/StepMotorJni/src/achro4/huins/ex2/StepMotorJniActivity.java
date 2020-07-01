package achro4.huins.ex2;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.SeekBar;
import android.widget.TextView;

public class StepMotorJniActivity extends Activity {
	
	private Button mSetStartBT; // Start
	private Button mSetStopBT; // Stop 
	private RadioGroup mRadioGroupDirection; // Direction 
	private SeekBar mSeekBarSpeed; // 0 ~ 253 Speed
	private RadioGroup mRadioGroupChangeSpeed; // 254, 255 
	private TextView mTextViewSpeed; 
	
	// Now Motor State Label
	private TextView mTextViewNMState;
	private TextView mTextViewNMDirection;
	private TextView mTextViewNMSpeed;
	
	private int setDirection;
	private int setSpeed;
	private String labelDirection; 
	private String labelSpeed; 
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		// Connect button and resource ID
		mSetStartBT = (Button)findViewById(R.id.BT01);
		mSetStopBT = (Button)findViewById(R.id.BT02);
		mRadioGroupDirection = (RadioGroup)findViewById(R.id.DirectionGroup00);
		mSeekBarSpeed = (SeekBar)findViewById(R.id.SpeedBar);
		mRadioGroupChangeSpeed = (RadioGroup)findViewById(R.id.ChangeSpeedGroup00);
		mTextViewSpeed = (TextView)findViewById(R.id.SpeedLabel);
		mTextViewNMState = (TextView)findViewById(R.id.NMState);
		mTextViewNMDirection = (TextView)findViewById(R.id.NMDirection);
		mTextViewNMSpeed = (TextView)findViewById(R.id.NMSpeed);
	
		
		// Process resource's event
		// Start Button is Active State So, first Parameter is 1
		mSetStartBT.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View vw) {  
				mTextViewNMState.setText("Motor State : Start");
				mTextViewNMDirection.setText("Motor Direction : " + labelDirection);
				mTextViewNMSpeed.setText("Motor Speed : " + labelSpeed);
				ReceiveStepMotorValue(1, setDirection, setSpeed); // View Now State
				mRadioGroupDirection.clearCheck();
				mRadioGroupChangeSpeed.clearCheck();
				mSeekBarSpeed.setProgress(0);  // State Clear
			}
		});

		// Process resource's event
		// Stop Button is Stop State So, first Parameter is 0
		mSetStopBT.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View vw) {
				mTextViewNMState.setText("Motor State : Stop");
				mTextViewNMDirection.setText("Motor Direction : " + labelDirection);
				mTextViewNMSpeed.setText("Motor Speed : " + labelSpeed); // View Now State
				ReceiveStepMotorValue(0, setDirection, setSpeed);
				mRadioGroupDirection.clearCheck();
				mRadioGroupChangeSpeed.clearCheck();
				mSeekBarSpeed.setProgress(0);  // State Clear
			}
		});
		
		// set Direction Listener
		mRadioGroupDirection.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub
				switch(checkedId)
				{
				case R.id.CLOCK :
					setDirection = 0; // Clock Direction
					labelDirection = "CLOCK";
					break ;    	
				case R.id.ACLOCK :
					setDirection = 1; // Counter Clock Direction
					labelDirection = "ANTICLOCK";
					break ;    	
				case R.id.CTOAC :
					setDirection = 2; //  Clock to Counter Clock
					labelDirection = "CLOCK TO ANTI";
					break ;    	
				case R.id.ACTOC :
					setDirection = 3; //  Counter Clock to Clock
					labelDirection = "ANTI TO CLOCK";
					break ;					
				}
			}
		});
		
		// 0 ~ 253 Speed Listener 
		mSeekBarSpeed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			// End Changed SeekBar Value 
			@Override
			public void onStopTrackingTouch(SeekBar arg0) {
				// TODO Auto-generated method stub
				
			}
			// Start Change SeekBar Value
			@Override
			public void onStartTrackingTouch(SeekBar arg0) {
				// TODO Auto-generated method stub
				
			}
			// Changed SeekBar Value 
			@Override
			public void onProgressChanged(SeekBar arg0, int arg1, boolean arg2) {
				// TODO Auto-generated method stub
				mRadioGroupChangeSpeed.clearCheck();
				setSpeed = arg1;
				mTextViewSpeed.setText("Speed : " + Integer.toString(setSpeed));
				labelSpeed = Integer.toString(setSpeed);
			}
		});

		// set 254, 255 Speed Listener
		mRadioGroupChangeSpeed.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub
				switch(checkedId)
				{
				case R.id.ltof :
					// progress zero
					mSeekBarSpeed.setProgress(0);
					setSpeed = 254; // Low To Fast Speed 
					mTextViewSpeed.setText("Low To Fast : " + Integer.toString(setSpeed));
					labelSpeed = Integer.toString(setSpeed);
					break ;    	
				case R.id.ftol :
					// progress zero
					mSeekBarSpeed.setProgress(0);
					setSpeed = 255; // Fast To Low Speed
					mTextViewSpeed.setText("Fast To Low : " + Integer.toString(setSpeed));
					labelSpeed = Integer.toString(setSpeed);
					break ;    			
				}
			}
		});
	} // end of OnCreate

	public native String ReceiveStepMotorValue(int action, int direction, int speed);

	static {
		System.loadLibrary("stepmotor-jni");
	}
}