using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CommandPanelController : MonoBehaviour
{
	[SerializeField] private GameObject commandPanelUnit;
	[SerializeField] private GameObject commandPanelFactory;
	[SerializeField] private GameObject commandPanelProducer;
	[SerializeField] private GameObject productionChangeDialogue;

	private GameObject activePanel;
	private SelectableController selected;

	private void Start()
	{
		this.activePanel = null;
		this.selected = null;
	}

	private void OnEnable()
	{
		SelectableManager.OnObjectSelect += this.OnObjectSelect;
		SelectableManager.OnObjectDeselect += this.OnObjectDeselect;
	}

	private void OnDisable()
	{
		SelectableManager.OnObjectSelect -= this.OnObjectSelect;
		SelectableManager.OnObjectDeselect -= this.OnObjectDeselect;
	}

	private void OnTurn()
	{
		this.OnObjectSelect(this.selected);
	}

	private void OnObjectSelect(SelectableController s)
	{
		if (this.activePanel != null)
		{
			this.activePanel.SetActive(false);
			this.activePanel = null;
		}

		if (this.selected == null)
		{
			this.selected = s;

			TurnManager.AddTurnEvent(this.OnTurn, TurnStage.UI);
		}

		Component c = null;

		//c = s.GetComponent<UnitController>()
		//c = s.GetComponent<FactoryController>()
		c = s.GetComponent<ResourceProducerController>();

		if (c != null)
		{
			this.UpdateProducerPanel((ResourceProducerController)c);
			return;
		}

		c = s.GetComponent<UnitController>();

		if (c != null)
		{
			this.UpdateUnitPanel((UnitController)c);
			return;
		}

		c = s.GetComponent<FactoryController>();

		if (c != null)
		{
			this.UpdateFactoryPanel((FactoryController)c);
			return;
		}
	}

	private void OnObjectDeselect()
	{
		if (this.activePanel != null)
		{
			this.activePanel.SetActive(false);
			this.activePanel = null;
		}

		if (this.selected != null)
		{
			this.selected = null;
			TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.UI);
		}
	}

	public void UpdateUnitPanel(UnitController unit)
	{
		this.commandPanelUnit.SetActive(true);
		this.activePanel = this.commandPanelUnit;
	}

	public void UpdateProducerPanel(ResourceProducerController producer)
	{
		this.commandPanelProducer.SetActive(true);
		this.activePanel = this.commandPanelProducer;

		for (int i = 0; i < producer.GetLevel(); i++)
		{
			Text t = this.activePanel.transform.GetChild(i).GetComponentInChildren<Text>();

			t.text = producer.GetResource(i).ToString() + "\n" + producer.GetAmountPerTurn(i).ToString();
		}

		for (int i = producer.GetLevel(); i < 3; i++)
		{
			Text t = this.activePanel.transform.GetChild(i).GetComponentInChildren<Text>();

			t.text = "Requires level " + (i + 1);
		}

		Button b = this.activePanel.GetComponentInChildren<Button>();
		b.onClick.RemoveAllListeners();

		Text bt = b.GetComponentInChildren<Text>();

		if (producer.GetTeam() != Team.Team1)
		{
			bt.text = "Capture This Building to Upgrade";
		}
		else
		{
			b.onClick.AddListener(producer.Upgrade);
			b.onClick.AddListener(() => this.UpdateProducerPanel(producer));

			switch (producer.GetLevel())
			{
				case 1:
					bt.text = "Upgrade to Level 2:\n\n30 Steel";
					break;
				case 2:
					bt.text = "Upgrade to Level 3:\n\n40 Oil";
					break;
				default:
					bt.text = "Max Upgrade Reached";
					break;
			}
		}
	}

	public void UpdateFactoryPanel(FactoryController c)
	{
		this.commandPanelFactory.SetActive(true);
		this.activePanel = this.commandPanelFactory;

		Transform infoBox = this.commandPanelFactory.transform.GetChild(2);
		Text infoText = infoBox.GetComponentInChildren<Text>();

		infoText.text = c.GetProductionInfo();

		Transform upgradeBox = this.commandPanelFactory.transform.GetChild(1);
		Text upgradeText = upgradeBox.GetComponentInChildren<Text>();
		Button upgradeButton = upgradeBox.GetComponent<Button>();

		Transform changeBox = this.commandPanelFactory.transform.GetChild(0);
		Button changeButton = changeBox.GetComponent<Button>();

		upgradeButton.onClick.RemoveAllListeners();

		changeButton.onClick.RemoveAllListeners();

		if (c.GetTeam() != Team.Team1)
		{
			upgradeText.text = "Capture This Building to Upgrade";

			upgradeButton.interactable = false;
			changeButton.interactable = false;
		}
		else
		{
			upgradeButton.onClick.AddListener(c.Upgrade);
			upgradeButton.onClick.AddListener(() => this.UpdateFactoryPanel(c));

			changeButton.onClick.AddListener(() => this.ShowProductionDialogue(c));
			changeButton.onClick.AddListener(() => this.UpdateFactoryPanel(c));

			upgradeButton.interactable = true;
			changeButton.interactable = true;

			switch (c.GetLevel())
			{
				case 1:
					upgradeText.text = "Upgrade to Level 2:\n\n20 Oil";
					break;
				case 2:
					upgradeText.text = "Upgrade to Level 3:\n\n10 Uranium";
					break;
				default:
					upgradeText.text = "Max Upgrade Reached";
					break;
			}
		}
	}

	private void ShowProductionDialogue(FactoryController c)
	{
		this.productionChangeDialogue.SetActive(true);

		for (int i = 0; i < 6; i++)
		{
			Transform tr = this.productionChangeDialogue.transform.GetChild(0).GetChild(i);
			Button b = tr.GetComponent<Button>();
			Text t = tr.GetComponentInChildren<Text>();

			Recipe r = FactoryManager.GetRecipe(i);

			b.onClick.RemoveAllListeners();

			if (c.GetLevel() >= r.levelRequired)
			{
				b.interactable = true;
				b.onClick.AddListener(() => c.ChangeProduction(r));
				b.onClick.AddListener(this.OnTurn);
				b.onClick.AddListener(this.HideProductionDialogue);
			}
			else
			{
				b.interactable = false;
			}

			t.text = r.name + "\n" + r.turnLength.ToString() + " Turns\n\n";

			foreach (RecipeCost rc in r.costs)
			{
				t.text += rc.amount.ToString() + " " + rc.resource.ToString() + "\n";
			}
		}

		Transform closeTransform = this.productionChangeDialogue.transform.GetChild(0).GetChild(6);
		Button closeButton = closeTransform.GetComponent<Button>();

		closeButton.onClick.AddListener(this.HideProductionDialogue);
	}

	private void HideProductionDialogue()
	{
		this.productionChangeDialogue.SetActive(false);
	}
}
